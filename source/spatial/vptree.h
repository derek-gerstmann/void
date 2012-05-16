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

#ifndef VD_SPATIAL_VPTREE_INCLUDED
#define VD_SPATIAL_VPTREE_INCLUDED

// ============================================================================================== //

#include "spatial/spatial.h"

#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <queue>
#include <limits>

// ============================================================================================== //

VD_SPATIAL_NAMESPACE_BEGIN();

// ============================================================================================== //

VD_IMPORT(Core, Function);

// ============================================================================================== //
// Based on "Data Structures and Algorithms for Nearest Neighbor Search" by Peter N. Yianilos
// Derived from Steve Hanov's implementation which was released to the Public Domain
// ============================================================================================== //

template<typename T, typename Real>
class VpTree
{
public:

    typedef Function< Real (const T&, const T&) > DistanceFn;

    VpTree(DistanceFn fn) : m_Root(0), m_DistanceFn(fn) {}

    ~VpTree()
    {
        VD_DELETE(m_Root);
    }

    void Create(const std::vector& entries)
    {
        VD_DELETE(m_Root);
        m_Entries = entries;
        m_Root = Build(0, items.size());
    }

    void Search(
        const T& target, vd::i32 k,
        std::vector* results,
        std::vector<Real>* distances)
    {
        std::priority_queue<HeapItem> heap;

        m_Tau = Real(vd::PosInf);
        Search(m_Root, target, k, heap);

        results->clear();
        distances->clear();

        while(!heap.empty())
        {
            results->push_back(m_Entries[heap.top().index]);
            distances->push_back(heap.top().dist);
            heap.pop();
        }

        std::reverse(results->begin(), results->end());
        std::reverse(distances->begin(), distances->end());
    }

private:

    struct Node
    {
        vd::i32 Index;
        Real Threshold;
        Node* Left;
        Node* Right;

        Node() :
            Index(0), 
            Threshold(vd::Zero), 
            Left(NULL), 
            Right(NULL) 
        {
        	// EMPTY
        }

        ~Node()
        {
            VD_DELETE(Left);
            VD_DELETE(Right);
        }
    };


    struct HeapItem
    {
        HeapItem(
        	vd::i32 index, 
        	Real delta
        ) :
            Index(index), 
            Delta(dist) 
        {
        	// EMPTY!
        }

        bool operator<(const HeapItem& o) const
        {
            return Delta < o.Delta;
        }

        vd::i32 Index;
        Real Delta;
    };

    struct DistanceComparator
    {
        const T& Item;
        const DistanceFn& Distance;

        DistanceComparator(
            const T& item,
            const DistanceFn& fn
        ) :
            Item(item), Distance(fn)
        {

        }

        bool operator()(const T& a, const T& b)
        {
            return Distance(Item, a) < Distance(Item, b);
        }
    };

    Node* Build(vd::i32 lower, vd::i32 upper)
    {
        if(upper == lower)
        {
            return NULL;
        }

        Node* node = new Node();
        node->Index = lower;

        if(upper - lower > 1)
        {
            vd::i32 i = (vd::i32)((Real)rand() / RAND_MAX * (upper - lower - 1)) + lower;
            std::swap(m_Entries[lower], m_Entries[i]);

            vd::i32 median = (upper + lower) / 2;

            std::nth_element(
                m_Entries.begin() + lower + 1,
                m_Entries.begin() + median,
                m_Entries.begin() + upper,
                DistanceComparator(m_Entries[lower], m_DistanceFn));

            node->Threshold = m_DistanceFn(m_Entries[lower], m_Entries[median]);
            node->Index = lower;
            node->Left = Build(lower + 1, median);
            node->Right = Build(median, upper);
        }

        return node;
    }

    void Search(
    	Node* node, const T& target, vd::i32 k,
		std::priority_queue& heap)
    {
        if(node == NULL) return;

        Real delta = m_DistanceFn(m_Entries[node->Index], target);
        if(delta < m_Tau)
        {
            if(heap.size() == k)
                heap.pop();

            heap.push(HeapItem(node->Index, delta));

            if(heap.size() == k)
                m_Tau = heap.top().Delta;
        }

        if(node->Left == NULL && node->Right == NULL)
        {
            return;
        }

        if(delta < node->Threshold)
        {
            if(delta - m_Tau <= node->Threshold)
            {
                Search(node->left, target, k, heap);
            }

            if(delta + m_Tau >= node->Threshold)
            {
                Search(node->Right, target, k, heap);
            }

        }
        else
        {
            if(delta + m_Tau >= node->Threshold)
            {
                Search(node->Right, target, k, heap);
            }

            if(delta - m_Tau <= node->Threshold)
            {
                Search(node->Left, target, k, heap);
            }
        }
    }

    Real m_Tau;
    Node* m_Root;
    std::vector<T> m_Entries;
    DistanceFn m_DistanceFn;
};

// ============================================================================================== //

typedef VpTree<vd::v2f32, vd::f32> VpTreeV2F32;
typedef VpTree<vd::v3f32, vd::f32> VpTreeV3F32;
typedef VpTree<vd::v4f32, vd::f32> VpTreeV4F32;

// ============================================================================================== //

VD_SPATIAL_NAMESPACE_END();

// ============================================================================================== //

#endif // VD_SPATIAL_VPTREE_INCLUDED
