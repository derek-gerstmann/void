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

#ifndef VD_SPATIAL_KDTREE_INCLUDED
#define VD_SPATIAL_KDTREE_INCLUDED

// ============================================================================================== //

#include "spatial/spatial.h"

// ============================================================================================== //

VD_SPATIAL_NAMESPACE_BEGIN();

// ============================================================================================== //
//
// KD-Tree based on Wenzel Jakob et al's excellent implementation.  Their license follows:
//
// ============================================================================================== //
//
// Copyright (c) 2007-2011 by Wenzel Jakob and others.
//
// Mitsuba is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License Version 3
// as published by the Free Software Foundation.
//
// Mitsuba is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// ============================================================================================== //

template <typename PT, typename DR> 
struct KDNodeBase
{
    typedef PT                       		 PointType;
    typedef DR                      		 DataRecord;
    typedef vd::u32                          IndexType;
    typedef typename PointType::ScalarType   ScalarType;

    enum
    {
        LeafFlag  =  0x10,
        AxisMask  =  0x0F
    };

    static const bool UseLeftBalancedTree = false;

    PointType Position;
    IndexType Right;
    DataRecord Data;
    vd::u8 Flags;

    inline KDNodeBase() : 
    	Position((ScalarType) 0),
        Right(0), Data(), Flags(0)
    { }
    
    inline KDNodeBase(const DataRecord& data) : Position((ScalarType) 0),
        Right(0), Data(data), Flags(0) { }

    inline IndexType GetRightIndex(IndexType self) const { return Right; }
    inline void SetRightIndex(IndexType self, IndexType value) { Right = value; }

    inline IndexType GetLeftIndex(IndexType self) const { return self + 1; }
    inline void SetLeftIndex(IndexType self, IndexType value)
    {
#if defined(VD_DEBUG)

        if(value != self + 1)
            vdLogGlobalError("KDNodeBase::SetLeftIndex(): Internal error!");

#endif
    }

    inline bool IsLeafNode() const { return Flags & (vd::u8) LeafFlag; }
    inline void SetLeafNode(bool value)
    {
        if(value)
            Flags |= (vd::u8) LeafFlag;
        else
            Flags &= (vd::u8) ~LeafFlag;
    }

    inline vd::u16 GetAxis() const { return Flags & (vd::u8) AxisMask; }
    inline void SetAxis(vd::u8 axis) { Flags = (Flags & (vd::u8) ~AxisMask) | axis; }

    inline const PointType& GetPosition() const { return Position; }
    inline void SetPosition(const PointType& value) { Position = value; }

    inline DataRecord& GetData() { return Data; }
    inline const DataRecord& GetData() const { return Data; }
    inline void SetData(const DataRecord& val) { Data = val; }
};

// ============================================================================================== //

template <typename PT, typename DR> 
struct LeftBalancedKDNode
{
    typedef PT                       		 PointType;
    typedef DR                      		 DataRecord;
    typedef vd::u32                          IndexType;
    typedef typename PointType::ScalarType   ScalarType;

    enum
    {
        LeafFlag  =  0x10,
        AxisMask  =  0x0F
    };

    static const bool UseLeftBalancedTree = true;

    PointType Position;
    DataRecord Data;
    vd::u8 Flags;

    inline LeftBalancedKDNode() : Position((ScalarType) 0), Data(), Flags(0) { }
    inline LeftBalancedKDNode(const DataRecord& data) : Position((ScalarType) 0),
        Data(data), Flags(0) { }

    inline IndexType GetLeftIndex(IndexType self) const { return 2 * self + 1; }
    inline void SetLeftIndex(IndexType self, IndexType value)
    {
#if defined(VD_DEBUG)

        if(value != 2 * self + 1)
            vdLogGlobalError("LeftBalancedKDNode::SetLeftIndex(): Internal error!");

#endif
    }

    inline IndexType GetRightIndex(IndexType self) const { return 2 * self + 2; }
    inline void SetRightIndex(IndexType self, IndexType value)
    {
#if defined(VD_DEBUG)

        if(value != 0 && value != 2 * self + 2)
            vdLogGlobalError("LeftBalancedKDNode::SetRightIndex(): Internal error!");

#endif
    }

    inline bool IsLeafNode() const { return Flags & (vd::u8) LeafFlag; }
    inline void SetLeafNode(bool value)
    {
        if(value)
            Flags |= (vd::u8) LeafFlag;
        else
            Flags &= (vd::u8) ~LeafFlag;
    }

    inline vd::u16 GetAxis() const { return Flags & (vd::u8) AxisMask; }
    inline void SetAxis(vd::u8 axis) { Flags = (Flags & (vd::u8) ~AxisMask) | axis; }

    inline const PointType& GetPosition() const { return Position; }
    inline void SetPosition(const PointType& value) { Position = value; }

    inline DataRecord& GetData() { return Data; }
    inline const DataRecord& GetData() const { return Data; }
    inline void SetData(const DataRecord& val) { Data = val; }
};

// ============================================================================================== //

template <typename NT> 
class PointKDTree
{
public:
    typedef NT                        		NodeType;
    typedef typename NodeType::PointType    PointType;
    typedef typename NodeType::IndexType    IndexType;
    typedef typename PointType::ScalarType  ScalarType;
    typedef typename PointType::VectorType  VectorType;
    typedef Region<PointType>               RegionType;

    VD_DECLARE_ENUM(Heuristic,
        Balanced,
		LeftBalanced,
		SlidingMidpoint,
		VoxelVolume
    };

    struct SearchResult
    {
        vd::real Delta;
        IndexType Index;

        inline SearchResult() {}

        inline SearchResult(vd::real delta, IndexType index)
            : Delta(delta), Index(index) { }

        std::string ToString() const
        {
            std::ostringstream oss;
            oss << "SearchResult[distance=" << std::sqrt(Delta)
                << ", index=" << Index << "]";
            return oss.str();
        }

        inline bool operator==(const SearchResult& r) const
        {
            return Delta == r.Delta &&
                   Index == r.Index;
        }
    };

    struct SearchResultComparator : public std::binary_function<SearchResult, SearchResult, bool>
    {
    public:
        inline bool operator()(const SearchResult& a, const SearchResult& b) const
        {
            return a.Delta < b.Delta;
        }
    };

public:
    
    inline PointKDTree(
        size_t nodes = 0, 
        Heuristic::Value heuristic = ESlidingMidpoint
    ) : m_Nodes(nodes), m_Heuristic(heuristic), m_Depth(0) { }

    inline void clear() { m_Nodes.clear(); m_Region.Reset(); }
    inline void resize(size_t size) { m_Nodes.resize(size); }
    inline void reserve(size_t size) { m_Nodes.reserve(size); }
    inline size_t size() const { return m_Nodes.size(); }
    inline size_t capacity() const { return m_Nodes.capacity(); }
    inline void push_back(const NodeType& node)
    {
        m_Nodes.push_back(node);
        m_Region.ExpandBy(node.GetPosition());
    }

    inline NodeType& operator[](size_t idx) { return m_Nodes[idx]; }
    inline const NodeType& operator[](size_t idx) const { return m_Nodes[idx]; }

    inline void SetRegion(const RegionType& aabb) { m_Region = aabb; }
    inline const RegionType& GetRegion() const { return m_Region; }
    inline size_t GetDepth() const { return m_Depth; }
    inline void SetDepth(size_t depth) { m_Depth = depth; }

    void Build(
        bool recompute = false)
    {
        Handle<Timer> timer = VD_NEW(Timer);

        if(m_Nodes.size() == 0)
        {
           vdLogGlobalWarning("Build(): kd-tree is empty!");
            return;
        }

        vdLogGlobalInfo("Building a %i-dimensional kd-tree over " SIZE_T_FMT " data points (%s)",
             PointType::dim, m_Nodes.size(), memString(m_Nodes.size() * sizeof(NodeType)).c_str());

        if(recompute)
        {
            m_Region.Reset();

            for(size_t i = 0; i < m_Nodes.size(); ++i)
                m_Region.ExpandBy(m_Nodes[i].GetPosition());

        }

        vd::i32 aabbTime = timer->GetMilliseconds();
        timer->Reset();

        std::vector<IndexType> indirection(m_Nodes.size());

        for(size_t i = 0; i < m_Nodes.size(); ++i)
            indirection[i] = i;

        m_Depth = 0;
        vd::i32 constructionTime;

        if(NodeType::UseLeftBalancedTree)
        {
            std::vector<IndexType> permutation(m_Nodes.size());
            BuildLeftBalanced(0, 1, indirection.begin(), indirection.begin(),
                    indirection.end(), permutation);
            constructionTime = timer->GetMilliseconds();
            timer->Reset();
            permute_inplace(&m_Nodes[0], permutation);
        }
        else
        {
            Build(1, indirection.begin(), indirection.begin(), indirection.end());
            constructionTime = timer->GetMilliseconds();
            timer->Reset();
            permute_inplace(&m_Nodes[0], indirection);
        }

        vd::i32 permutationTime = timer->GetMilliseconds();

        if(recompute)
            vdLogGlobalInfo("Done after %i ms (breakdown: aabb: %i ms, Build: %i ms, permute: %i ms). ",
                 aabbTime + constructionTime + permutationTime, aabbTime, constructionTime, permutationTime);
        else
            vdLogGlobalInfo("Done after %i ms (breakdown: Build: %i ms, permute: %i ms). ",
                 constructionTime + permutationTime, constructionTime, permutationTime);
    }

    size_t 
    Search(
        const PointType& p, 
        vd::real& _sqrSearchRadius,
        size_t k, SearchResult* results) const
    {
        if(m_Nodes.size() == 0)
            return 0;

        IndexType* stack = (IndexType*) alloca((m_Depth + 1) * sizeof(IndexType));
        IndexType index = 0, stackPos = 1;
        vd::real sqrSearchRadius = _sqrSearchRadius;
        size_t resultCount = 0;
        bool isHeap = false;
        stack[0] = 0;

        while(stackPos > 0)
        {
            const NodeType& node = m_Nodes[index];
            IndexType next_index;

            /* Recurse on inner nodes */
            if(!node.IsLeafNode())
            {
                vd::real planar_delta = p[node.GetAxis()] - node.GetPosition()[node.GetAxis()];

                bool full_search = planar_delta * planar_delta <= sqrSearchRadius;

                if(planar_delta > 0)
                {
                    /* The search query is located on the Right side of the split.
                       Search this side first. */
                    if(HasRightChild(index))
                    {
                        if(full_search)
                            stack[stackPos++] = node.GetLeftIndex(index);

                        next_index = node.GetRightIndex(index);
                    }
                    else if(full_search)
                    {
                        next_index = node.GetLeftIndex(index);
                    }
                    else
                    {
                        next_index = stack[--stackPos];
                    }
                }
                else
                {
                    /* The search query is located on the left side of the split.
                       Search this side first. */
                    if(full_search && HasRightChild(index))
                        stack[stackPos++] = node.GetRightIndex(index);

                    next_index = node.GetLeftIndex(index);
                }
            }
            else
            {
                next_index = stack[--stackPos];
            }

            const vd::real delta = DistanceSqr(node.GetPosition(), p);
            if(delta < sqrSearchRadius)
            {
                if(resultCount < k)
                {
                    results[resultCount++] = SearchResult(delta, index);
                }
                else
                {
                    if(!isHeap)
                    {
                        /* Establish the max-heap property */
                        std::make_heap(results, results + resultCount,
                                       SearchResultComparator());
                        isHeap = true;
                    }

                    SearchResult* end = results + resultCount + 1;

                    results[resultCount] = SearchResult(delta, index);
                    std::push_heap(results, end, SearchResultComparator());
                    std::pop_heap(results, end, SearchResultComparator());
                    sqrSearchRadius = results[0].Delta;
                }
            }

            index = next_index;
        }

        _sqrSearchRadius = sqrSearchRadius;
        return resultCount;
    }

    size_t ProfiledSearch(
    	const PointType& p, 
    	vd::real& sqrSearchRadius, size_t k, 
        SearchResult* results, size_t& traversalSteps) const
    {
        traversalSteps = 0;
        if(m_Nodes.size() == 0)
            return 0;

        IndexType* stack = (IndexType*) alloca((m_Depth + 1) * sizeof(IndexType));
        IndexType index = 0, stackPos = 1;
        size_t resultCount = 0;
        bool isHeap = false;
        stack[0] = 0;

        while(stackPos > 0)
        {
            const NodeType& node = m_Nodes[index];
            ++traversalSteps;
            IndexType next_index;

            if(!node.IsLeafNode())
            {
                vd::real planar_delta = p[node.GetAxis()] - node.GetPosition()[node.GetAxis()];
                bool full_search = planar_delta * planar_delta <= sqrSearchRadius;
                if(planar_delta > 0)
                {
                    if(HasRightChild(index))
                    {
                        if(full_search)
                            stack[stackPos++] = node.GetLeftIndex(index);

                        next_index = node.GetRightIndex(index);
                    }
                    else if(full_search)
                    {
                        next_index = node.GetLeftIndex(index);
                    }
                    else
                    {
                        next_index = stack[--stackPos];
                    }
                }
                else
                {
                    if(full_search && HasRightChild(index))
                        stack[stackPos++] = node.GetRightIndex(index);

                    next_index = node.GetLeftIndex(index);
                }
            }
            else
            {
                next_index = stack[--stackPos];
            }

            const vd::real delta = DistanceSqr(node.GetPosition(), p);
            if(delta < sqrSearchRadius)
            {
                if(resultCount < k)
                {
                    results[resultCount++] = SearchResult(delta, index);
                }
                else
                {
                    if(!isHeap)
                    {
                        std::make_heap(results, results + resultCount, SearchResultComparator());
                        isHeap = true;
                    }

                    results[resultCount] = SearchResult(delta, index);
                    std::push_heap(results, results + resultCount + 1, SearchResultComparator());
                    std::pop_heap(results, results + resultCount + 1, SearchResultComparator());
                    sqrSearchRadius = results[0].Delta;
                }
            }

            index = next_index;
        }

        return resultCount;
    }

    inline 
    size_t Search(
    	const PointType& p, size_t k,
       	SearchResult* results) const
    {
        vd::real delta = std::numeric_limits<vd::real>::infinity();
        return Search(p, delta, k, results);
    }

    template <typename Functor> 
    size_t Execute(
    	const PointType& p,
		vd::real search_radius, 
		Functor& functor)
    {
        if(m_Nodes.size() == 0)
            return 0;

        IndexType* stack = (IndexType*) alloca((m_Depth + 1) * sizeof(IndexType));
        size_t index = 0, stackPos = 1, found = 0;
        vd::real delta = search_radius * search_radius;
        stack[0] = 0;

        while(stackPos > 0)
        {
            NodeType& node = m_Nodes[index];
            IndexType next_index;

            if(!node.IsLeafNode())
            {
                vd::real planar_delta = p[node.GetAxis()] - node.GetPosition()[node.GetAxis()];
                bool full_search = planar_delta * planar_delta <= delta;
                if(planar_delta > 0)
                {
                    if(HasRightChild(index))
                    {
                        if(full_search)
                            stack[stackPos++] = node.GetLeftIndex(index);

                        next_index = node.GetRightIndex(index);
                    }
                    else if(full_search)
                    {
                        next_index = node.GetLeftIndex(index);
                    }
                    else
                    {
                        next_index = stack[--stackPos];
                    }
                }
                else
                {
                    if(full_search && HasRightChild(index))
                        stack[stackPos++] = node.GetRightIndex(index);

                    next_index = node.GetLeftIndex(index);
                }
            }
            else
            {
                next_index = stack[--stackPos];
            }

            const vd::real dsqr = DistanceSqr(node.GetPosition(), p);
            if(dsqr < delta)
            {
                functor(node);
                ++found;
            }

            index = next_index;
        }

        return found;
    }

    template <typename Functor> size_t 
    ExecuteQuery(
    	const PointType& p,
        vd::real search_radius, 
        Functor& functor) const
    {
        if(m_Nodes.size() == 0)
            return 0;

        IndexType* stack = (IndexType*) alloca((m_Depth + 1) * sizeof(IndexType));
        size_t index = 0, stackPos = 1, found = 0;
        vd::real delta = search_radius * search_radius;
        stack[0] = 0;

        while(stackPos > 0)
        {
            const NodeType& node = m_Nodes[index];
            IndexType next_index;

            if(!node.IsLeafNode())
            {
                vd::real planar_delta = p[node.GetAxis()] - node.GetPosition()[node.GetAxis()];

                bool full_search = planar_delta * planar_delta <= delta;

                if(planar_delta > 0)
                {
                    if(HasRightChild(index))
                    {
                        if(full_search)
                            stack[stackPos++] = node.GetLeftIndex(index);

                        next_index = node.GetRightIndex(index);
                    }
                    else if(full_search)
                    {
                        next_index = node.GetLeftIndex(index);
                    }
                    else
                    {
                        next_index = stack[--stackPos];
                    }
                }
                else
                {
                    if(full_search && HasRightChild(index))
                        stack[stackPos++] = node.GetRightIndex(index);

                    next_index = node.GetLeftIndex(index);
                }
            }
            else
            {
                next_index = stack[--stackPos];
            }

            const vd::real dsqr = DistanceSqr(node.GetPosition(), p);
            if(dsqr < delta)
            {
                ++found;
                functor(node);
            }

            index = next_index;
        }

        return found;
    }

    size_t 
    Search(
    	const PointType& p, 
    	vd::real search_radius, 
    	std::vector<IndexType> &results) const
    {
        if(m_Nodes.size() == 0)
            return 0;

        IndexType* stack = (IndexType*) alloca((m_Depth + 1) * sizeof(IndexType));
        size_t index = 0, stackPos = 1, found = 0;
        vd::real delta = search_radius * search_radius;
        stack[0] = 0;

        while(stackPos > 0)
        {
            const NodeType& node = m_Nodes[index];
            IndexType next_index;

            if(!node.IsLeafNode())
            {
                vd::real planar_delta = p[node.GetAxis()] - node.GetPosition()[node.GetAxis()];

                bool full_search = planar_delta * planar_delta <= delta;

                if(planar_delta > 0)
                {
                    if(HasRightChild(index))
                    {
                        if(full_search)
                            stack[stackPos++] = node.GetLeftIndex(index);

                        next_index = node.GetRightIndex(index);
                    }
                    else if(full_search)
                    {
                        next_index = node.GetLeftIndex(index);
                    }
                    else
                    {
                        next_index = stack[--stackPos];
                    }
                }
                else
                {
                    if(full_search && HasRightChild(index))
                        stack[stackPos++] = node.GetRightIndex(index);

                    next_index = node.GetLeftIndex(index);
                }
            }
            else
            {
                next_index = stack[--stackPos];
            }

            const vd::real dsqr = DistanceSqr(node.GetPosition(), p);
            if(dsqr < delta)
            {
                ++found;
                results.push_back(index);
            }

            index = next_index;
        }

        return found;
    }

    inline bool HasRightChild(IndexType index) const
    {
        if(NodeType::UseLeftBalancedTree)
        {
            return 2 * index + 2 < m_Nodes.size();
        }
        else
        {
            return m_Nodes[index].GetRightIndex(index) != 0;
        }
    }
protected:
    struct CoordinateOrdering : public std::binary_function<IndexType, IndexType, bool>
    {
    public:
        inline CoordinateOrdering(std::vector<NodeType> &nodes, int axis)
            : m_Nodes(nodes), m_Axis(axis) { }
        inline bool operator()(const IndexType& i1, const IndexType& i2) const
        {
            return m_Nodes[i1].GetPosition()[m_Axis] < m_Nodes[i2].GetPosition()[m_Axis];
        }
    private:
        std::vector<NodeType> &m_Nodes;
        int m_Axis;
    };

    struct LessThanOrEqual : public std::unary_function<IndexType, bool>
    {
    public:
        inline LessThanOrEqual(std::vector<NodeType> &nodes, int axis, ScalarType value)
            : m_Nodes(nodes), m_Axis(axis), m_Value(value) { }
        inline bool operator()(const IndexType& i) const
        {
            return m_Nodes[i].GetPosition()[m_Axis] <= m_Value;
        }
    private:
        std::vector<NodeType> &m_Nodes;
        int m_Axis;
        ScalarType m_Value;
    };

    inline IndexType 
    FindLeftSubtreeSize(IndexType count) const
    {
        IndexType p = 1;
        while(2 * p <= count)
            p *= 2;

        IndexType remaining = count - p + 1;
        if(2 * remaining < p)
        {
            p = (p >> 1) + remaining;
        }

        return p - 1;
    }

    void 
    BuildLeftBalanced(
    	IndexType idx, size_t depth,
        typename std::vector<IndexType>::iterator base,
        typename std::vector<IndexType>::iterator range_start,
        typename std::vector<IndexType>::iterator range_end,
        typename std::vector<IndexType> &permutation)
    {
        m_Depth = std::max(depth, m_Depth);

        size_t count = range_end - range_start;
        vdGlobalAssert(count > 0);

        if(count == 1)
        {
            m_Nodes[*range_start].SetLeafNode(true);
            permutation[idx] = *range_start;
            return;
        }

        typename std::vector<IndexType>::iterator split = range_start + FindLeftSubtreeSize(count);
        int axis = m_Region.GetLargestAxis();

        std::nth_element(range_start, split, range_end, CoordinateOrdering(m_Nodes, axis));

        NodeType& split_node = m_Nodes[*split];
        split_node.SetAxis(axis);
        split_node.SetLeafNode(false);
        permutation[idx] = *split;

        ScalarType temp = m_Region.max[axis];
		ScalarType split_pos = split_node.GetPosition()[axis];
        m_Region.max[axis] = split_pos;
        BuildLeftBalanced(2 * idx + 1, depth + 1, base, range_start, split, permutation);
        m_Region.max[axis] = temp;

        if(split + 1 != range_end)
        {
            temp = m_Region.min[axis];
            m_Region.min[axis] = split_pos;
            BuildLeftBalanced(2 * idx + 2, depth + 1, base, split + 1, range_end, permutation);
            m_Region.min[axis] = temp;
        }
    }

    void 
    Build(
    	size_t depth,
		typename std::vector<IndexType>::iterator base,
		typename std::vector<IndexType>::iterator range_start,
		typename std::vector<IndexType>::iterator range_end)
    {
        m_Depth = std::max(depth, m_Depth);
        size_t count = range_end - range_start;
        vdGlobalAssert(count > 0);

        if(count == 1)
        {
            m_Nodes[*range_start].SetLeafNode(true);
            return;
        }

        int axis = 0;
        typename std::vector<IndexType>::iterator split;
        switch(m_Heuristic)
        {
            case Heuristic::Balanced:
            {
                split = range_start + count / 2;
                axis = m_Region.GetLargestAxis();
                std::nth_element(range_start, split, range_end, CoordinateOrdering(m_Nodes, axis));
            };

            break;

            case Heuristic::LeftBalanced:
            {
                split = range_start + FindLeftSubtreeSize(count);
                axis = m_Region.GetLargestAxis();
                std::nth_element(range_start, split, range_end, CoordinateOrdering(m_Nodes, axis));
            };

            break;

            case Heuristic::SlidingMidpoint:
            {
                axis = m_Region.GetLargestAxis();
                ScalarType midpoint = (ScalarType) 0.5f * (m_Region.max[axis] + m_Region.min[axis]);
                size_t nLT = std::count_if(range_start, range_end, LessThanOrEqual(m_Nodes, axis, midpoint));
                split = range_start + nLT;
                if(split == range_start)
                    ++split;
                else if(split == range_end)
                    --split;

                std::nth_element(range_start, split, range_end, CoordinateOrdering(m_Nodes, axis));
            };

            break;

            case Heuristic::VoxelVolume:
            {
                vd::real best_cost = std::numeric_limits<vd::real>::infinity();

                for(int dim = 0; dim < PointType::dim; ++dim)
                {
                    std::sort(range_start, range_end, CoordinateOrdering(m_Nodes, dim));
                    size_t left_count = 1;
                    size_t right_count = count - 2;
                    RegionType left_region(m_Region);
                    RegionType right_region(m_Region);
                    vd::real rcp_volume = 1.0f / m_Region.GetVolume();

                    for(typename std::vector<IndexType>::iterator it = range_start + 1;
                            it != range_end; ++it)
                    {
                        ++left_count; 
                        --right_count;
                        
                        vd::real pos = m_Nodes[*it].GetPosition()[dim];
                        left_region.max[dim] = right_region.min[dim] = pos;

                        vd::real cost = (left_count * left_region.GetVolume()
                                      + right_count * right_region.GetVolume()) * rcp_volume;

                        if(cost < best_cost)
                        {
                            best_cost = cost;
                            axis = dim;
                            split = it;
                        }
                    }
                }

                std::nth_element(range_start, split, range_end, CoordinateOrdering(m_Nodes, axis));
            };

            break;
        }

        NodeType& split_node = m_Nodes[*split];
        split_node.SetAxis(axis);
        split_node.SetLeafNode(false);

        if(split + 1 != range_end)
            split_node.SetRightIndex(range_start - base, split + 1 - base);
        else
            split_node.SetRightIndex(range_start - base, 0);

        split_node.SetLeftIndex(range_start - base, range_start + 1 - base);
        std::iter_swap(range_start, split);

        ScalarType temp = m_Region.max[axis];
        ScalarType split_pos = split_node.GetPosition()[axis];
        m_Region.max[axis] = split_pos;
        Build(depth + 1, base, range_start + 1, split + 1);
        m_Region.max[axis] = temp;

        if(split + 1 != range_end)
        {
            temp = m_Region.min[axis];
            m_Region.min[axis] = split_pos;
            Build(depth + 1, base, split + 1, range_end);
            m_Region.min[axis] = temp;
        }
    }
protected:
    std::vector<NodeType> m_Nodes;
    RegionType m_Region;
    Heuristic::Value m_Heuristic;
    size_t m_Depth;
};

// ============================================================================================== //

VD_SPATIAL_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_SPATIAL_KDTREE_INCLUDED

