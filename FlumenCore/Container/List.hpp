#pragma once

#include "Array.hpp"

namespace container
{
	template <class ObjectType>
	struct ListNode
	{
		ObjectType Object_;

		ListNode <ObjectType> * Previous_;

		ListNode <ObjectType> * Next_;

		ListNode() : Previous_(nullptr), Next_(nullptr) {}
	};

	template <class ObjectType>
	class StaticList
	{
		typedef ListNode <ObjectType> Node;

		Array <Node> nodes_;

		Node* lastNode_;

		StaticList()
		{
			lastNode_ = nullptr;
		}

		StaticList(int capacity)
		{
			Initialize(capacity);
		}

		void Initialize(int capacity)
		{
			nodes_.Initialize(capacity);

			lastNode_ = nullptr;
		}

		Node * Add()
		{
			auto node = nodes_.Allocate();
			if(!node)
				return nullptr;

			if(lastNode_)
			{
				lastNode_->Next_ = node;
			}
			node->Previous_ = lastNode_;

			lastNode_ = node;

			return node;
		}

		Node * GetFirstNode()
		{
			return nodes_.GetStart();
		}
	};
}
