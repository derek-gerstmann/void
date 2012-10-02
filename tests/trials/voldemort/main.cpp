// ============================================================================================== //
//
// License:		The Lesser GNU Public License (LGPL) v3.0.
// 
// Author(s): 	Derek Gerstmann. The University of Western Australia (UWA). 
//				As well as the shoulders of many giants...
//
// This file is part of the Void framework.
//
// The Void framework is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// The Void framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the Void framework.  If not, see <http://www.gnu.org/licenses/>.
//
// ============================================================================================== //

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <signal.h>
#include <memory.h>
#include <unistd.h>
#include <getopt.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include <pthread.h>
#include <voldemort/StoreClient.h>
#include <voldemort/StoreClientFactory.h>
#include <voldemort/SocketStoreClientFactory.h>
#include <voldemort/ClientConfig.h>
#include <sstream>
#include <memory>

// ============================================================================================== //

// #include "core/core.h"
// #include "core/registry.h"
// #include "test/test.h"
// #include "test/speed.h"

// ============================================================================================== //

using Voldemort::StoreClient;
using Voldemort::StoreClientFactory;
using Voldemort::SocketStoreClientFactory;
using Voldemort::ClientConfig;
using Voldemort::VersionedValue;

// ============================================================================================== //

extern char* optarg;
extern int optint, opterr, optopt;
const char* g_progname;

static const char* g_host = "127.0.0.1";
static unsigned short g_port = 6666;

static std::list<std::string> g_boostrap_urls;
static ClientConfig* g_config;
static StoreClientFactory* g_factory;

static std::string g_store_name = "test";

static unsigned long long g_num_request;
static bool g_keymode_64 = false;
static unsigned long g_num_thread = 1;
static unsigned long g_keylen = 16;
static unsigned long g_vallen = 1024;
static bool g_noset = false;
static bool g_noget = false;
static unsigned long long g_offset = 0;
static bool g_del = false;

static pthread_mutex_t g_count_lock;
static pthread_cond_t  g_count_cond;
static volatile unsigned int g_thread_count;
static pthread_mutex_t g_thread_lock;

#define KEY_FILL 'k'

static struct timeval g_timer;

// ============================================================================================== //

void reset_timer()
{
	gettimeofday(&g_timer, NULL);
}

void show_timer()
{
	struct timeval endtime;
	double sec;
	double requests = g_num_request * g_num_thread;
	double size_bytes = (g_keylen+g_vallen) * requests;
	gettimeofday(&endtime, NULL);
	sec = (endtime.tv_sec - g_timer.tv_sec)
		+ (double)(endtime.tv_usec - g_timer.tv_usec) / 1000 / 1000;
	printf("%lf sec\n",      sec);
	printf("%lf MB\n",       size_bytes/1024/1024);
	printf("%lf Mbps\n",     size_bytes*8/sec/1000/1000);
	printf("%lf req/sec\n",  requests/sec);
	printf("%lf usec/req\n", sec/requests*1000*1000);
}

// ============================================================================================== //

static pthread_t* create_worker(void* (*func)(void*))
{
	unsigned long i;
	pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t)*g_num_thread);

	pthread_mutex_lock(&g_thread_lock);
	g_thread_count = 0;

	for(i=0; i < g_num_thread; ++i) {
		int err = pthread_create(&threads[i], NULL, func, NULL);
		if(err != 0) {
			fprintf(stderr, "failed to create thread: %s\n", strerror(err));
			exit(1);
		}
	}

	pthread_mutex_lock(&g_count_lock);
	while(g_thread_count < g_num_thread) {
		pthread_cond_wait(&g_count_cond, &g_count_lock);
	}
	pthread_mutex_unlock(&g_count_lock);

	return threads;
}

static void start_worker()
{
	pthread_mutex_unlock(&g_thread_lock);
}

static void join_worker(pthread_t* threads)
{
	unsigned long i;
	for(i=0; i < g_num_thread; ++i) {
		void* ret;
		int err = pthread_join(threads[i], &ret);
		if(err != 0) {
			fprintf(stderr, "failed to join thread: %s\n", strerror(err));
		}
	}
}

static unsigned long wait_worker_ready()
{
	unsigned long index;
	pthread_mutex_lock(&g_count_lock);
	index = g_thread_count++;
	pthread_cond_signal(&g_count_cond);
	pthread_mutex_unlock(&g_count_lock);
	pthread_mutex_lock(&g_thread_lock);
	pthread_mutex_unlock(&g_thread_lock);
	return index;
}

// ============================================================================================== //

void initialize_global()
{
	g_config = new ClientConfig();

	std::ostringstream s;
	s << "tcp://"<<g_host<<":"<<g_port;
	g_boostrap_urls.push_back(s.str());

	g_config->setBootstrapUrls(&g_boostrap_urls);

	g_factory = new SocketStoreClientFactory(*g_config);
}

static StoreClient* initialize_user()
{
	return g_factory->getStoreClient(g_store_name);
}

static char* malloc_keybuf()
{
	char* keybuf = (char*)malloc(g_keylen+1);
	if(!keybuf) {
		perror("malloc for key failed");
		exit(1);
	}
	memset(keybuf, KEY_FILL, g_keylen);
	keybuf[g_keylen] = '\0';
	return keybuf;
}

static char* malloc_valbuf()
{
	char* valbuf = (char*)malloc(g_vallen);
	if(!valbuf) {
		perror("malloc for value failed");
		exit(1);
	}
	memset(valbuf, 'v', g_vallen);
	//memset(valbuf, 0, g_vallen);
	return valbuf;
}

static void pack_keynum(char* keybuf, unsigned long long i)
{
	i += g_offset;
	/* 0x40 - 0x4f is printable ascii character */
	if(g_keymode_64) {
		unsigned char* prefix = (unsigned char*)keybuf + g_keylen - 16;
		prefix[ 0] = ((i >> 0) & 0x0f) + 0x40;
		prefix[ 1] = ((i >> 4) & 0x0f) + 0x40;
		prefix[ 2] = ((i >> 8) & 0x0f) + 0x40;
		prefix[ 3] = ((i >>12) & 0x0f) + 0x40;
		prefix[ 4] = ((i >>16) & 0x0f) + 0x40;
		prefix[ 5] = ((i >>20) & 0x0f) + 0x40;
		prefix[ 6] = ((i >>24) & 0x0f) + 0x40;
		prefix[ 7] = ((i >>28) & 0x0f) + 0x40;
		prefix[ 8] = ((i >>32) & 0x0f) + 0x40;
		prefix[ 9] = ((i >>36) & 0x0f) + 0x40;
		prefix[10] = ((i >>40) & 0x0f) + 0x40;
		prefix[11] = ((i >>44) & 0x0f) + 0x40;
		prefix[12] = ((i >>48) & 0x0f) + 0x40;
		prefix[13] = ((i >>52) & 0x0f) + 0x40;
		prefix[14] = ((i >>56) & 0x0f) + 0x40;
		prefix[15] = ((i >>60) & 0x0f) + 0x40;
	} else {
		unsigned char* prefix = (unsigned char*)keybuf + g_keylen - 8;
		prefix[0] = ((i >> 0) & 0x0f) + 0x40;
		prefix[1] = ((i >> 4) & 0x0f) + 0x40;
		prefix[2] = ((i >> 8) & 0x0f) + 0x40;
		prefix[3] = ((i >>12) & 0x0f) + 0x40;
		prefix[4] = ((i >>16) & 0x0f) + 0x40;
		prefix[5] = ((i >>20) & 0x0f) + 0x40;
		prefix[6] = ((i >>24) & 0x0f) + 0x40;
		prefix[7] = ((i >>28) & 0x0f) + 0x40;
	}
}

static void* worker_set(void* trash)
{
	unsigned long long i, t;
	StoreClient* st = initialize_user();
	char* keybuf = malloc_keybuf();
	char* valbuf = malloc_valbuf();
	std::string value(valbuf, g_vallen);
	
	printf("s");
	t = wait_worker_ready();

	for(i=t*g_num_request, t=i+g_num_request; i < t; ++i) {
		pack_keynum(keybuf, i);
		std::string key(keybuf, g_keylen);
		try {
			st->put(&key, &value);
		} catch (std::exception& e) {
			fprintf(stderr, "set failed: %s\n", e.what());
		}
	}
	
	free(keybuf);
	free(valbuf);
	return NULL;
}

static void* worker_get(void* trash)
{
	unsigned long long i, t;
	StoreClient* st = initialize_user();

	printf("g");
	t = wait_worker_ready();

	char* keybuf = malloc_keybuf();

	for(i=t*g_num_request, t=i+g_num_request; i < t; ++i) {
		pack_keynum(keybuf, i);
		std::string key(keybuf, g_keylen);
		try {
			const VersionedValue* value = st->get(&key);
			if(!value) {
				fprintf(stderr, "get failed: key not found\n");
			}
		} catch (std::exception& e) {
			fprintf(stderr, "get failed: %s\n", e.what());
		}
	}

	free(keybuf);
	return NULL;
}


static void* worker_del(void* trash)
{
	unsigned long long i, t;
	StoreClient* st = initialize_user();
	char* keybuf = malloc_keybuf();

	printf("d");
	t = wait_worker_ready();

	for(i=t*g_num_request, t=i+g_num_request; i < t; ++i) {
		pack_keynum(keybuf, i);
		std::string key(keybuf, g_keylen);
		try {
			st->deleteKey(&key);
		} catch (std::exception& e) {
			fprintf(stderr, "del failed: %s\n", e.what());
		}
	}

	free(keybuf);
	return NULL;
}

// ============================================================================================== //

static void usage(const char* msg)
{
	printf("Usage: %s [options]    <num requests>\n"
		" -l HOST=127.0.0.1  : bootstrap server address\n"
		" -p PORT=6666       : bootstrap server port\n"
		" -a NAME=test       : store name\n"
		" -k SIZE=16         : size of key >= 8\n"
		" -v SIZE=1024       : size of value\n"
		" -m NUM=1           : get multiplex\n"
		" -t NUM=1           : number of threads\n"
		" -b                 : use binary protocol\n"
		" -g                 : omit to set values\n"
		" -s                 : omit to get benchmark\n"
		" -x                 : delete keys\n"
		" -o NUM=0           : key offset\n"
		" -h                 : print this help message\n"
		, g_progname);
	if(msg) { printf("error: %s\n", msg); }
	exit(1);
}

static void parse_argv(int argc, char* argv[])
{
	int c;
	g_progname = argv[0];
	while((c = getopt(argc, argv, "hbgsxl:p:a:k:v:m:t:o:d:")) != -1) {
		switch(c) {
		case 'l':
			g_host = optarg;
			break;

		case 'p':
			g_port = atoi(optarg);
			if(g_port == 0) { usage("invalid port number"); }
			break;

		case 'a':
			g_store_name = optarg;
			break;

		case 'k':
			g_keylen = strtol(optarg, NULL, 10);
			if(g_keylen < 8) { usage("invalid key size"); }
			break;

		case 'v':
			g_vallen  = strtol(optarg, NULL, 10);
			if(g_vallen == 0) { usage("invalid value size"); }
			break;

		case 't':
			g_num_thread  = strtol(optarg, NULL, 10);
			break;

		case 'g':
			g_noset = true;
			break;

		case 's':
			g_noget = true;
			break;

		case 'o':
			g_offset = strtoll(optarg, NULL, 10);
			if(g_offset == 0) { usage("invalid key offset"); }
			break;

		case 'x':
			g_del = true;
			break;

		case 'h': /* FALL THROUGH */
		case '?': /* FALL THROUGH */
		default:
			usage(NULL);
		}
	}
	
	argc -= optind;

	if(argc != 1) { usage(NULL); }

	char* numstr = argv[optind];
	long long int multiplex_request = strtoll(numstr, NULL, 10) / g_num_thread;

	g_num_request = multiplex_request;

	if(g_num_request == 0) { usage("invalid number of request"); }

	if(g_num_request > 0xffffffffLLU) {
		if(g_keylen < 16) {
			usage("size of key must be >= 16 if number of requests is >= 4294967295");
		}
		g_keymode_64 = true;
	}

	printf("number of threads    : %lu\n",  g_num_thread);
	printf("number of requests   : %llu\n", g_num_request * g_num_thread);
	printf("requests per thread  : %llu\n", g_num_request);
	printf("size of key          : %lu bytes\n", g_keylen);
	printf("size of value        : %lu bytes\n", g_vallen);
}

// ============================================================================================== //

int main(int argc, char* argv[])
{
	pthread_t* threads;

	parse_argv(argc, argv);	

	signal(SIGPIPE, SIG_IGN);

	pthread_mutex_init(&g_count_lock, NULL);
	pthread_cond_init(&g_count_cond, NULL);
	pthread_mutex_init(&g_thread_lock, NULL);

	initialize_global();

	if(!g_noset) {
		printf("----\n[");
		threads = create_worker(worker_set);
		reset_timer();
		printf("] ...\n");
		start_worker();
		join_worker(threads);
		show_timer();
	}

	if(!g_noget) {
		printf("----\n[");
		threads = create_worker(worker_get);
		reset_timer();
		printf("] ...\n");
		start_worker();
		join_worker(threads);
		show_timer();
	}

	if(g_del) {
		printf("----\n[");
		threads = create_worker(worker_del);
		reset_timer();
		printf("] ...\n");
		start_worker();
		join_worker(threads);
		show_timer();
	}

	return 0;
}

// ============================================================================================== //

