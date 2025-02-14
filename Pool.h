
#pragma once

/**
 * @brief Basic templated pool
 */
namespace example
{
	template< typename OBJECT >
	class Pool
	{
		typedef OBJECT Object;
		
		/**
		 * @brief Base node class
		 */
		class Node
		{
		public:
			/* Object of the node */
			Object object;

			/* Pointer to next node */
			Node * next;

			/* Pointer to previous node */
			Node * prev;
		};

		/* List of nodes */
		std::vector< Node > nodes;

		/* Pointer to the first free node */
		Node* first_free;

		/* Pointer to the first node in use */
		Node* first_used;

	public:
		/**
		 * @brief Iterator class of the nodes
		 */
		class Iterator
		{
			/* Pointer to the current node */
			Node* current;

		public:
			/* Constructor */
			Iterator(Node* start)
			{
				current = start;
			}

			/* Copy constructor by default */
			Iterator(const Iterator&) = default;

			/* Overloaded ++ operator to move over the nodes */
			Iterator& operator++ ()
			{
				current = current->next;
				return *this;
			}

			Iterator operator++ (int)
			{
				Iterator copy(*this);
				current = current->next;
				return copy;
			}

			/* Overloaded * operator to access the object */
			Object& operator* ()
			{
				return current->object;
			}

			/* Overloaded == operator to compare between objects */
			bool operator== (const Iterator & other) const
			{
				return this->current == other.current;
			}

			/* Overloaded != operator to compare between objects */
			bool operator!= (const Iterator & other) const
			{
				return this->current != other.current;
			}
		};

	public:
		/**
		 * @brief Pool Constructor
		 * @param size of the pool
		 */
		Pool(size_t size) : nodes(size)
		{
			if (size > 0)
			{
				first_free = &nodes[0];
			}
			else
			{
				first_free = nullptr;
			}

			if (size > 1)
			{
				first_free->next = &nodes[1];
			}
			first_free->prev = nullptr;

			first_used = nullptr;


			Node* current = first_free->next;

			for (int i = 1; i < nodes.size() -1; ++i)
			{
				current = &nodes[i];
				current->prev = &nodes[i - 1];
				current->next = &nodes[i + 1];
				current = current->next;
			}

			current = &nodes[nodes.size() - 1];
			current->prev = &nodes[nodes.size() - 2];
			current->next = nullptr;
		}

		/**
		 * @brief Returns a free object
		 * @return Object* 
		 */
		Object* get_free_object()
		{
			Node* free = first_free;
			if (free)
			{
				first_free = free->next;

				if (free->next)
				{
					free->next->prev = nullptr;
				}

				free->next = first_used;
				
				if (first_used)
				{
					first_used->prev = free;
				}
				
				first_used = free;
			}
			
			return reinterpret_cast<Object*>(free);
		}

		/**
		 * @brief Set the received object free
		 * @param object 
		 */
		void free_object(Object* object)
		{
			Node* node = reinterpret_cast<Node*>(object);

			if (node->prev)
			{
				node->prev->next = node->next;
			}
			else
			{
				first_used = node->next;
			}

			if (node->next)
			{
				node->next->prev = node->prev;
			}

			node->prev = nullptr;
			node->next = first_free;

			if (first_free)
			{
				first_free->prev = node;
			}

			first_free = node;
		}

		Iterator begin()
		{
			return Iterator(first_used);
		}

		Iterator end()
		{
			return Iterator(nullptr);
		}
	};
}

