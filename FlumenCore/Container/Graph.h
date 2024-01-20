#pragma once

#include "FlumenCore/Container/SmartBlock.hpp"
#include "FlumenCore/Container/Array.hpp"
#include "FlumenCore/Singleton.h"

namespace container
{
    template <typename Type, int NodeCapacity = 8>
    class Graph
    {
    public:
        struct Node;

    private:
        class EndNodeBuffer : public core::Singleton <EndNodeBuffer>
        {
            friend class Node;
             
            friend class Graph;

            Array <Node *> endNodes;

        public:
            EndNodeBuffer()
            {
                endNodes.Initialize(65536);
            }
        };

    public:
        struct Node
        {
            friend class Graph;

        private:
            const Graph *Parent;

            Node *PreviousNode;

            SmartBlock <Node *, NodeCapacity> NextNodes;

            void CheckFinality()
            {
                auto &nodes = EndNodeBuffer::Get()->endNodes;

                if(NextNodes.GetSize() == 0)
                {
                    *nodes.Add() = this;
                }
                else
                {
                    for(auto node = NextNodes.GetFirst(); node != NextNodes.GetLast(); ++node)
                    {
                        (*node)->CheckFinality();
                    }
                }
            }

        public:
            Type Content;

            Node(Type content, const Graph *parent, Node *previousNode = nullptr) : Content(content), Parent(parent), PreviousNode(previousNode)
            {
                NextNodes.Clear();
            }

            Node *AddNode(Type content)
            {
                auto &nodes = Parent->nodes;
                auto address = nodes.Add();

                *address = Node(content, Parent, this);

                *NextNodes.Add() = address;

                return address;
            }

            Node *GetPrevious() const
            {
                return PreviousNode;
            }

            const SmartBlock <Node *, NodeCapacity> &GetLinks()
            {
                return NextNodes;
            }
        };

    private:
        Node *firstNode;

        Array <Node> nodes;

    public:
        Graph() {}

        Graph(int size)
        {
            nodes.Initialize(size);

            firstNode = nodes.GetStart();
        }

        Graph(int size, Type content)
        {
            nodes.Initialize(size);

            firstNode = nodes.GetStart();

            StartGraph(content);
        }

        Node *StartGraph(Type content)
        {
            *nodes.Add() = Node(content, this, nullptr);

            return firstNode;
        }

        void Clear()
        {
            nodes.Reset();
        }

        int GetSize()
        {
            return nodes.GetSize();;
        }

        const Array <Node> &GetNodes()
        {
            return nodes;
        }

        const Array <Node *> &GetEndNodes()
        {
            auto &nodes = EndNodeBuffer::Get()->endNodes;

            nodes.Reset();

            firstNode->CheckFinality();

            return nodes;
        }
    };
}