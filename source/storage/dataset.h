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

#ifndef VD_DATASET_HEADER_INCLUDED
#define VD_DATASET_HEADER_INCLUDED

// ============================================================================================== //

#include "storage/storage.h"

#include "core/core.h"
#include "core/cache.h"
#include "core/collections.h"
#include "core/object.h"
#include "core/memory.h"
#include "core/atomics.h"
#include "core/workqueue.h"

// ============================================================================================== //

VD_STORAGE_NAMESPACE_BEGIN();

// ============================================================================================== //

class DataObject : public Core::Object
{
    DataObject();    
    virtual ~DataObject();
	
	virtual vd::status 
	Destroy();

	VD_DECLARE_OBJECT(DataGroup);
	
private:

	VD_DISABLE_COPY_CONSTRUCTORS(DataGroup);
};

// ============================================================================================== //

class DataGroup : public DataObject
{
    DataGroup();    
    virtual ~DataGroup();
	
	virtual vd::status 
	Destroy();

	VD_DECLARE_OBJECT(DataGroup);
	
private:

	VD_DISABLE_COPY_CONSTRUCTORS(DataGroup);
};

// ============================================================================================== //

class DataSet : public Core::Object
{
public:
    DataSet();    
    virtual ~DataSet();
	
	virtual void 
	Destroy();
	
	virtual vd::status 
	Open(const vd::string& container, const vd::string& dataset, const vd::string& options) = 0;
	
	virtual vd::status 
	Close() = 0;
	
	virtual vd::uid 
	Resolve(const vd::string& parent, const vd::string& path) = 0;

	virtual vd::status
	IsSupported(const Core::Symbol feature) = 0;

	virtual vd::status
	IsValidSourceFormat(const vd::string& uri) = 0;

	virtual DataGroup* 
	Load(vd::uid index) = 0;
	
	virtual DataGroup* 
	Fetch(vd::uid index) = 0;

	virtual vd::status 
	Request(const DataRequest& request) = 0;

	virtual vd::u64 GetEntryCount() const = 0;
	virtual vd::uid GetStartIndex() const = 0;
	virtual vd::uid GetEndIndex() const  = 0;
	virtual vd::uid GetCurrentIndex() const  = 0;
	virtual void SetCurrentIndex(vd::uid index) = 0;

	virtual bool IsOpen() const = 0;
	virtual bool IsResident(vd::i32 index) const = 0;
	virtual bool IsPending(vd::i32 index) const = 0;
	virtual bool IsReady(vd::i32 index) const = 0;

	VD_DECLARE_OBJECT(DataSet);
	
private:

	VD_DISABLE_COPY_CONSTRUCTORS(DataSet);
};

// ============================================================================================== //

VD_STORAGE_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_DATASET_HEADER_INCLUDED

// ============================================================================================== //
// END OF FILE

