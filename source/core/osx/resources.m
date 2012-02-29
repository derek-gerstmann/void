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

#import <Cocoa/Cocoa.h>
#import <wchar.h>
#import <pthread.h>

static const char* ResourcePath = 0;
static const char* DesktopPath = 0;
static pthread_key_t AutoReleaseKey;
	
static void 
vdCreateAutoReleasePool()
{
	pthread_key_create(&AutoReleaseKey, NULL);
}

static void 
vdDestroyAutoReleasePool() 
{
	pthread_key_delete(AutoReleaseKey);
}

static void 
vdFillAutoReleasePool() 
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	pthread_setspecific(AutoReleaseKey, pool);
}

static void 
vdDrainAutoReleasePool() 
{
	NSAutoreleasePool *pool = (NSAutoreleasePool *)(pthread_getspecific(AutoReleaseKey));
	[pool release]; 
}


void
vdStartupResources()
{
	vdCreateAutoReleasePool();
	vdFillAutoReleasePool();
}

void
vdShutdownResources()
{
	vdDrainAutoReleasePool();
	vdDestroyAutoReleasePool();
}

const char* 
vdGetDesktopFolder()
{
	if(DesktopPath)
		return DesktopPath;

    NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDesktopDirectory, NSUserDomainMask, NO);
    DesktopPath = [[paths objectAtIndex:0] UTF8String];
	return DesktopPath;
}

const char* 
vdGetResourceFolder()
{
	if(ResourcePath)
		return ResourcePath;

    ResourcePath = [[[NSBundle mainBundle] resourcePath] UTF8String];
	return ResourcePath;
}

const char* 
vdOpenFileDialog()
{
	const char* filepath = 0;

    NSOpenPanel *oPanel = [NSOpenPanel openPanel];
//  NSArray *fileTypes = [NSArray arrayWithObjects:@"raw", @"hdr", @"bov", nil];
//	[oPanel setAllowedFileTypes:fileTypes];
    int result = [oPanel runModal];
    if (result == NSOKButton) 
    {
        NSArray *filesToOpen = [oPanel URLs];
        NSURL *fileURL = [filesToOpen objectAtIndex:0];
        NSString* strPath = [fileURL absoluteString];
        NSString* strLocal = @"file://localhost/";
        NSString* strRoot = @"/";        
        NSString* result = [strPath stringByReplacingOccurrencesOfString:strLocal withString:strRoot];
		filepath = [result UTF8String];
    }
    return filepath;
}

