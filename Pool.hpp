/**
 * @brief Pool básica
 * 
 */


namespace example
{
	template< typename OBJECT >
	class Pool
	{
		typedef OBJECT Object;
		
		/**
		 * @brief Clase Nodo
		 * 
		 */
		class Node
		{
		public:
			/**
			 * @brief Objeto
			 * 
			 */
			Object object;

			/**
			 * @brief Nodo anterior
			 * 
			 */
			Node * next;

			/**
			 * @brief Nodo siguiente
			 * 
			 */
			Node * prev;
		};

		/**
		 * @brief Vector de nodos
		 * 
		 */
		std::vector< Node > nodes;

		/**
		 * @brief Apunta al primer elemento de los nodos sin usar
		 * 
		 */
		Node * first_free;

		/**
		 * @brief Apunta al primer elemento de los nodos en uso
		 * 
		 */
		Node * first_used;


	public:

		/**
		 * @brief Clase iterador entre nodos
		 * 
		 */
		class Iterator
		{
			Node * current;

		public:

			Iterator(Node * start)
			{
				current = start;
			}

			Iterator(const Iterator &) = default;

			Iterator & operator ++ ()
			{
				current = current->next;
				return *this;
			}

			Iterator operator ++ (int)
			{
				Iterator copy(*this);
				current = current->next;
				return copy;
			}

			Object & operator * ()
			{
				return current->object;
			}

			bool operator == (const Iterator & other) const
			{
				return this->current == other.current;
			}

			bool operator != (const Iterator & other) const
			{
				return this->current != other.current;
			}
		};

	public:

		/**
		 * @brief Constructor de Pool
		 * 
		 * @param size 
		 */
		Pool(size_t size) : nodes(size)
		{
			if (size > 0)
				this->first_free = &nodes[0];
			else
				first_free = nullptr;

			if (size > 1)
			{
				first_free->next = &nodes[1];
			}
			first_free->prev = nullptr;

			this->first_used = nullptr;


			Node * current = first_free->next;

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
		 * @brief Devuelve Un objeto sin uso
		 * 
		 * @return Object* 
		 */
		Object * get_free_object()
		{
			Node * free = first_free;

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
			
			return reinterpret_cast<Object *>(free);

		}

		/**
		 * @brief Livera el objeto que recibe
		 * 
		 * @param object 
		 */
		void free_object(Object * object)
		{
			Node * node = reinterpret_cast<Node *>(object);

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

			if(first_free)
				first_free->prev = node;

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

