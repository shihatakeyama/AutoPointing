// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// rapidxml.cpp
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"

#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>


//--#include "rapidxml_define.h"
//#include "rapidxml_if.h"

//#include "rapidxml.h"
#include "rapidxml.h"



//! Class representing a node of XML document. 
//! Each node may have associated name and value strings, which are available through name() and value() functions. 
//! Interpretation of name and value depends on type of the node.
//! Type of node can be determined by using type() function.
//! <br><br>
//! Note that after parse, both name and value of node, if any, will point interior of source text used for parsing. 
//! Thus, this text must persist in the memory for the lifetime of node.
//! \param Ch Character type to use.
//--template<class Ch = char>
//--class xml_node : public xml_base<Ch>
//--{

//--public:

namespace rapidxml{

	typedef TCHAR Ch;



	///////////////////////////////////////////////////////////////////////
	// Memory pool

	//! This class is used by the parser to create new nodes and attributes, without overheads of dynamic memory allocation.
	//! In most cases, you will not need to use this class directly. 
	//! However, if you need to create nodes manually or modify names/values of nodes, 
	//! you are encouraged to use memory_pool of relevant xml_document to allocate the memory. 
	//! Not only is this faster than allocating them by using <code>new</code> operator, 
	//! but also their lifetime will be tied to the lifetime of document, 
	//! possibly simplyfing memory management. 
	//! <br><br>
	//! Call allocate_node() or allocate_attribute() functions to obtain new nodes or attributes from the pool. 
	//! You can also call allocate_string() function to allocate strings.
	//! Such strings can then be used as names or values of nodes without worrying about their lifetime.
	//! Note that there is no <code>free()</code> function -- all allocations are freed at once when clear() function is called, 
	//! or when the pool is destroyed.
	//! <br><br>
	//! It is also possible to create a standalone memory_pool, and use it 
	//! to allocate nodes, whose lifetime will not be tied to any document.
	//! <br><br>
	//! Pool maintains <code>RAPIDXML_STATIC_POOL_SIZE</code> bytes of statically allocated memory. 
	//! Until static memory is exhausted, no dynamic memory allocations are done.
	//! When static memory is exhausted, pool allocates additional blocks of memory of size <code>RAPIDXML_DYNAMIC_POOL_SIZE</code> each,
	//! by using global <code>new[]</code> and <code>delete[]</code> operators. 
	//! This behaviour can be changed by setting custom allocation routines. 
	//! Use set_allocator() function to set them.
	//! <br><br>
	//! Allocations for nodes, attributes and strings are aligned at <code>RAPIDXML_ALIGNMENT</code> bytes.
	//! This value defaults to the size of pointer on target architecture.
	//! <br><br>
	//! To obtain absolutely top performance from the parser,
	//! it is important that all nodes are allocated from a single, contiguous block of memory.
	//! Otherwise, cache misses when jumping between two (or more) disjoint blocks of memory can slow down parsing quite considerably.
	//! If required, you can tweak <code>RAPIDXML_STATIC_POOL_SIZE</code>, <code>RAPIDXML_DYNAMIC_POOL_SIZE</code> and <code>RAPIDXML_ALIGNMENT</code> 
	//! to obtain best wasted memory to performance compromise.
	//! To do it, define their values before rapidxml.hpp file is included.
	//! \param Ch Character type of created nodes. 
//--	template<class Ch = char>
//--	class memory_pool
//--	{

//--	public:

		//! \cond internal
//--		typedef void *(alloc_func)(std::size_t);       // Type of user-defined function used to allocate memory
//--		typedef void (free_func)(void *);              // Type of user-defined function used to free memory
		//! \endcond

		//! Constructs empty pool with default allocator functions.
		template <>
		memory_pool<Ch>::memory_pool()
			: m_alloc_func(0)
			, m_free_func(0)
		{
			init();
		}

		//! Destroys pool and frees all the memory. 
		//! This causes memory occupied by nodes allocated by the pool to be freed.
		//! Nodes allocated from the pool are no longer valid.
		template <>
		memory_pool<Ch>::~memory_pool()
		{
			clear();
		}

		//! Allocates a new node from the pool, and optionally assigns name and value to it. 
		//! If the allocation request cannot be accomodated, this function will throw <code>std::bad_alloc</code>.
		//! If exceptions are disabled by defining RAPIDXML_NO_EXCEPTIONS, this function
		//! will call rapidxml::parse_error_handler() function.
		//! \param type Type of node to create.
		//! \param name Name to assign to the node, or 0 to assign no name.
		//! \param value Value to assign to the node, or 0 to assign no value.
		//! \param name_size Size of name to assign, or 0 to automatically calculate size from name string.
		//! \param value_size Size of value to assign, or 0 to automatically calculate size from value string.
		//! \return Pointer to allocated node. This pointer will never be NULL.
		template <>
		xml_node<Ch> *memory_pool<Ch>::allocate_node(node_type type,
			const Ch *name, const Ch *value,
			std::size_t name_size, std::size_t value_size)
		{
			void *memory = allocate_aligned(sizeof(xml_node<Ch>));
			xml_node<Ch> *node = new(memory)xml_node<Ch>(type);
			if (name)
			{
				if (name_size > 0)
					node->name(name, name_size);
				else
					node->name(name);
			}
			if (value)
			{
				if (value_size > 0)
					node->value(value, value_size);
				else
					node->value(value);
			}
			return node;
		}

		//! Allocates a new attribute from the pool, and optionally assigns name and value to it.
		//! If the allocation request cannot be accomodated, this function will throw <code>std::bad_alloc</code>.
		//! If exceptions are disabled by defining RAPIDXML_NO_EXCEPTIONS, this function
		//! will call rapidxml::parse_error_handler() function.
		//! \param name Name to assign to the attribute, or 0 to assign no name.
		//! \param value Value to assign to the attribute, or 0 to assign no value.
		//! \param name_size Size of name to assign, or 0 to automatically calculate size from name string.
		//! \param value_size Size of value to assign, or 0 to automatically calculate size from value string.
		//! \return Pointer to allocated attribute. This pointer will never be NULL.
		template <>
		xml_attribute<Ch> *memory_pool<Ch>::allocate_attribute(const Ch *name, const Ch *value,
			std::size_t name_size, std::size_t value_size)
		{
			void *memory = allocate_aligned(sizeof(xml_attribute<Ch>));
			xml_attribute<Ch> *attribute = new(memory)xml_attribute<Ch>;
			if (name)
			{
				if (name_size > 0)
					attribute->name(name, name_size);
				else
					attribute->name(name);
			}
			if (value)
			{
				if (value_size > 0)
					attribute->value(value, value_size);
				else
					attribute->value(value);
			}
			return attribute;
		}

		//! Allocates a char array of given size from the pool, and optionally copies a given string to it.
		//! If the allocation request cannot be accomodated, this function will throw <code>std::bad_alloc</code>.
		//! If exceptions are disabled by defining RAPIDXML_NO_EXCEPTIONS, this function
		//! will call rapidxml::parse_error_handler() function.
		//! \param source String to initialize the allocated memory with, or 0 to not initialize it.
		//! \param size Number of characters to allocate, or zero to calculate it automatically from source string length; if size is 0, source string must be specified and null terminated.
		//! \return Pointer to allocated char array. This pointer will never be NULL.
		template <>
		Ch *memory_pool<Ch>::allocate_string(const Ch *source, std::size_t size)
		{
			assert(source || size);     // Either source or size (or both) must be specified
			if (size == 0)
				size = internal::measure(source) + 1;
			Ch *result = static_cast<Ch *>(allocate_aligned(size * sizeof(Ch)));
			if (source)
				for (std::size_t i = 0; i < size; ++i)
					result[i] = source[i];
			return result;
		}

		//! Clones an xml_node and its hierarchy of child nodes and attributes.
		//! Nodes and attributes are allocated from this memory pool.
		//! Names and values are not cloned, they are shared between the clone and the source.
		//! Result node can be optionally specified as a second parameter, 
		//! in which case its contents will be replaced with cloned source node.
		//! This is useful when you want to clone entire document.
		//! \param source Node to clone.
		//! \param result Node to put results in, or 0 to automatically allocate result node
		//! \return Pointer to cloned node. This pointer will never be NULL.
		template <>
		xml_node<Ch> *memory_pool<Ch>::clone_node(const xml_node<Ch> *source, xml_node<Ch> *result)
		{
			// Prepare result node
			if (result)
			{
				result->remove_all_attributes();
				result->remove_all_nodes();
				result->type(source->type());
			}
			else
				result = allocate_node(source->type());

			// Clone name and value
			result->name(source->name(), source->name_size());
			result->value(source->value(), source->value_size());

			// Clone child nodes and attributes
			for (xml_node<Ch> *child = source->first_node(); child; child = child->next_sibling())
				result->append_node(clone_node(child));
			for (xml_attribute<Ch> *attr = source->first_attribute(); attr; attr = attr->next_attribute())
				result->append_attribute(allocate_attribute(attr->name(), attr->value(), attr->name_size(), attr->value_size()));

			return result;
		}

		//! Clears the pool. 
		//! This causes memory occupied by nodes allocated by the pool to be freed.
		//! Any nodes or strings allocated from the pool will no longer be valid.
		template <>
		void memory_pool<Ch>::clear()
		{
			while (m_begin != m_static_memory)
			{
				char *previous_begin = reinterpret_cast<header *>(align(m_begin))->previous_begin;
				if (m_free_func)
					m_free_func(m_begin);
				else
					delete[] m_begin;
				m_begin = previous_begin;
			}
			init();
		}


		//! Sets or resets the user-defined memory allocation functions for the pool.
		//! This can only be called when no memory is allocated from the pool yet, otherwise results are undefined.
		//! Allocation function must not return invalid pointer on failure. It should either throw,
		//! stop the program, or use <code>longjmp()</code> function to pass control to other place of program. 
		//! If it returns invalid pointer, results are undefined.
		//! <br><br>
		//! User defined allocation functions must have the following forms:
		//! <br><code>
		//! <br>void *allocate(std::size_t size);
		//! <br>void free(void *pointer);
		//! </code><br>
		//! \param af Allocation function, or 0 to restore default function
		//! \param ff Free function, or 0 to restore default function
		template <>
		void memory_pool<Ch>::set_allocator(alloc_func *af, free_func *ff)
		{
			assert(m_begin == m_static_memory && m_ptr == align(m_begin));    // Verify that no memory is allocated yet
			m_alloc_func = af;
			m_free_func = ff;
		}

		template <>
		void memory_pool<Ch>::init()
		{
			m_begin = m_static_memory;
			m_ptr = align(m_begin);
			m_end = m_static_memory + sizeof(m_static_memory);
		}

		template <>
		char *memory_pool<Ch>::align(char *ptr)
		{
			std::size_t alignment = ((RAPIDXML_ALIGNMENT - (std::size_t(ptr) & (RAPIDXML_ALIGNMENT - 1))) & (RAPIDXML_ALIGNMENT - 1));
			return ptr + alignment;
		}

		template <>
		char *memory_pool<Ch>::allocate_raw(std::size_t size)
		{
			// Allocate
			void *memory;
			if (m_alloc_func)   // Allocate memory using either user-specified allocation function or global operator new[]
			{
				memory = m_alloc_func(size);
				assert(memory); // Allocator is not allowed to return 0, on failure it must either throw, stop the program or use longjmp
			}
			else
			{
				memory = new char[size];
#ifdef RAPIDXML_NO_EXCEPTIONS
				if (!memory)            // If exceptions are disabled, verify memory allocation, because new will not be able to throw bad_alloc
					RAPIDXML_PARSE_ERROR("out of memory", 0);
#endif
			}
			return static_cast<char *>(memory);
		}

		template <>
		void *memory_pool<Ch>::allocate_aligned(std::size_t size)
		{
			// Calculate aligned pointer
			char *result = align(m_ptr);

			// If not enough memory left in current pool, allocate a new pool
			if (result + size > m_end)
			{
				// Calculate required pool size (may be bigger than RAPIDXML_DYNAMIC_POOL_SIZE)
				std::size_t pool_size = RAPIDXML_DYNAMIC_POOL_SIZE;
				if (pool_size < size)
					pool_size = size;

				// Allocate
				std::size_t alloc_size = sizeof(header) + (2 * RAPIDXML_ALIGNMENT - 2) + pool_size;     // 2 alignments required in worst case: one for header, one for actual allocation
				char *raw_memory = allocate_raw(alloc_size);

				// Setup new pool in allocated memory
				char *pool = align(raw_memory);
				header *new_header = reinterpret_cast<header *>(pool);
				new_header->previous_begin = m_begin;
				m_begin = raw_memory;
				m_ptr = pool + sizeof(header);
				m_end = raw_memory + alloc_size;

				// Calculate aligned pointer again using new pool
				result = align(m_ptr);
			}

			// Update pool and return aligned pointer
			m_ptr = result + size;
			return result;
		}





	///////////////////////////////////////////////////////////////////////////
	// XML base

	//! Base class for xml_node and xml_attribute implementing common functions: 
	//! name(), name_size(), value(), value_size() and parent().
	//! \param Ch Character type to use
//	template<class Ch = char>
//	class xml_base
//	{

//	public:

//ss	template <class Ch>
//ss	xml_node<Ch>::xml_node(node_type type)


	///////////////////////////////////////////////////////////////////////////
	// Construction & destruction
	// Construct a base with empty name, value and parent
	template <>
	xml_base<Ch>::xml_base()
		: m_name(0)
		, m_value(0)
		, m_parent(0)
	{
	}

	///////////////////////////////////////////////////////////////////////////
	// Node data access

	//! Gets name of the node. 
	//! Interpretation of name depends on type of node.
	//! Note that name will not be zero-terminated if rapidxml::parse_no_string_terminators option was selected during parse.
	//! <br><br>
	//! Use name_size() function to determine length of the name.
	//! \return Name of node, or empty string if node has no name.
	template <>
	Ch *xml_base<Ch>::name() const
	{
		return m_name ? m_name : nullstr();
	}

	//! Gets size of node name, not including terminator character.
	//! This function works correctly irrespective of whether name is or is not zero terminated.
	//! \return Size of node name, in characters.
	template <>
	std::size_t xml_base<Ch>::name_size() const
	{
		return m_name ? m_name_size : 0;
	}

	//! Gets value of node. 
	//! Interpretation of value depends on type of node.
	//! Note that value will not be zero-terminated if rapidxml::parse_no_string_terminators option was selected during parse.
	//! <br><br>
	//! Use value_size() function to determine length of the value.
	//! \return Value of node, or empty string if node has no value.
	template <>
	Ch *xml_base<Ch>::value() const
	{
		return m_value ? m_value : nullstr();
	}

	//! Gets size of node value, not including terminator character.
	//! This function works correctly irrespective of whether value is or is not zero terminated.
	//! \return Size of node value, in characters.
	template <>
	std::size_t xml_base<Ch>::value_size() const
	{
		return m_value ? m_value_size : 0;
	}

	///////////////////////////////////////////////////////////////////////////
	// Node modification

	//! Sets name of node to a non zero-terminated string.
	//! See \ref ownership_of_strings.
	//! <br><br>
	//! Note that node does not own its name or value, it only stores a pointer to it. 
	//! It will not delete or otherwise free the pointer on destruction.
	//! It is reponsibility of the user to properly manage lifetime of the string.
	//! The easiest way to achieve it is to use memory_pool of the document to allocate the string -
	//! on destruction of the document the string will be automatically freed.
	//! <br><br>
	//! Size of name must be specified separately, because name does not have to be zero terminated.
	//! Use name(const Ch *) function to have the length automatically calculated (string must be zero terminated).
	//! \param name Name of node to set. Does not have to be zero terminated.
	//! \param size Size of name, in characters. This does not include zero terminator, if one is present.
	template <>
	void xml_base<Ch>::name(const Ch *name, std::size_t size)
	{
		m_name = const_cast<Ch *>(name);
		m_name_size = size;
	}

	//! Sets name of node to a zero-terminated string.
	//! See also \ref ownership_of_strings and xml_node::name(const Ch *, std::size_t).
	//! \param name Name of node to set. Must be zero terminated.
	template <>
	void xml_base<Ch>::name(const Ch *name)
	{
		this->name(name, internal::measure(name));
	}

	//! Sets value of node to a non zero-terminated string.
	//! See \ref ownership_of_strings.
	//! <br><br>
	//! Note that node does not own its name or value, it only stores a pointer to it. 
	//! It will not delete or otherwise free the pointer on destruction.
	//! It is reponsibility of the user to properly manage lifetime of the string.
	//! The easiest way to achieve it is to use memory_pool of the document to allocate the string -
	//! on destruction of the document the string will be automatically freed.
	//! <br><br>
	//! Size of value must be specified separately, because it does not have to be zero terminated.
	//! Use value(const Ch *) function to have the length automatically calculated (string must be zero terminated).
	//! <br><br>
	//! If an element has a child node of type node_data, it will take precedence over element value when printing.
	//! If you want to manipulate data of elements using values, use parser flag rapidxml::parse_no_data_nodes to prevent creation of data nodes by the parser.
	//! \param value value of node to set. Does not have to be zero terminated.
	//! \param size Size of value, in characters. This does not include zero terminator, if one is present.
	template <>
	void xml_base<Ch>::value(const Ch *value, std::size_t size)
	{
		m_value = const_cast<Ch *>(value);
		m_value_size = size;
	}

	//! Sets value of node to a zero-terminated string.
	//! See also \ref ownership_of_strings and xml_node::value(const Ch *, std::size_t).
	//! \param value Vame of node to set. Must be zero terminated.
	template <>
	void xml_base<Ch>::value(const Ch *value)
	{
		this->value(value, internal::measure(value));
	}

	///////////////////////////////////////////////////////////////////////////
	// Related nodes access

	//! Gets node parent.
	//! \return Pointer to parent node, or 0 if there is no parent.
	template <>
	xml_node<Ch> *xml_base<Ch>::parent() const
	{
		return m_parent;
	}

	// Return empty string
	template <>
	static Ch *xml_base<Ch>::nullstr()
	{
		static Ch zero = Ch('\0');
		return &zero;
	}


	///////////////////////////////////////////////////////////////////////////
	// xml_attribute

	//! Class representing attribute node of XML document. 
	//! Each attribute has name and value strings, which are available through name() and value() functions (inherited from xml_base).
	//! Note that after parse, both name and value of attribute will point to interior of source text used for parsing. 
	//! Thus, this text must persist in memory for the lifetime of attribute.
	//! \param Ch Character type to use.


		///////////////////////////////////////////////////////////////////////////
		// Construction & destruction

		//! Constructs an empty attribute with the specified type. 
		//! Consider using memory_pool of appropriate xml_document if allocating attributes manually.
		template <>
		xml_attribute<Ch>::xml_attribute()
		{
		}

		///////////////////////////////////////////////////////////////////////////
		// Related nodes access

		//! Gets document of which attribute is a child.
		//! \return Pointer to document that contains this attribute, or 0 if there is no parent document.
		template <>
		xml_document<Ch> *xml_attribute<Ch>::document() const
		{
			if (xml_node<Ch> *node = this->parent())
			{
				while (node->parent())
					node = node->parent();
				return node->type() == node_document ? static_cast<xml_document<Ch> *>(node) : 0;
			}
			else
				return 0;
		}

		//! Gets previous attribute, optionally matching attribute name. 
		//! \param name Name of attribute to find, or 0 to return previous attribute regardless of its name; this string doesn't have to be zero-terminated if name_size is non-zero
		//! \param name_size Size of name, in characters, or 0 to have size calculated automatically from string
		//! \param case_sensitive Should name comparison be case-sensitive; non case-sensitive comparison works properly only for ASCII characters
		//! \return Pointer to found attribute, or 0 if not found.
		template <>
		xml_attribute<Ch> *xml_attribute<Ch>::previous_attribute(const Ch *name, std::size_t name_size, bool case_sensitive) const
		{
			if (name)
			{
				if (name_size == 0)
					name_size = internal::measure(name);
				for (xml_attribute<Ch> *attribute = m_prev_attribute; attribute; attribute = attribute->m_prev_attribute)
					if (internal::compare(attribute->name(), attribute->name_size(), name, name_size, case_sensitive))
						return attribute;
				return 0;
			}
			else
				return this->m_parent ? m_prev_attribute : 0;
		}

		//! Gets next attribute, optionally matching attribute name. 
		//! \param name Name of attribute to find, or 0 to return next attribute regardless of its name; this string doesn't have to be zero-terminated if name_size is non-zero
		//! \param name_size Size of name, in characters, or 0 to have size calculated automatically from string
		//! \param case_sensitive Should name comparison be case-sensitive; non case-sensitive comparison works properly only for ASCII characters
		//! \return Pointer to found attribute, or 0 if not found.
		template <>
		xml_attribute<Ch> *xml_attribute<Ch>::next_attribute(const Ch *name, std::size_t name_size, bool case_sensitive) const
		{
			if (name)
			{
				if (name_size == 0)
					name_size = internal::measure(name);
				for (xml_attribute<Ch> *attribute = m_next_attribute; attribute; attribute = attribute->m_next_attribute)
					if (internal::compare(attribute->name(), attribute->name_size(), name, name_size, case_sensitive))
						return attribute;
				return 0;
			}
			else
				return this->m_parent ? m_next_attribute : 0;
		}


	///////////////////////////////////////////////////////////////////////////
	// XML node

	///////////////////////////////////////////////////////////////////////////
	// Construction & destruction

	//! Constructs an empty node with the specified type. 
	//! Consider using memory_pool of appropriate document to allocate nodes manually.
	//! \param type Type of node to construct.
	template <>
	xml_node<Ch>::xml_node(node_type type)
		: m_type(type)
		, m_first_node(0)
		, m_first_attribute(0)
	{
	}

	///////////////////////////////////////////////////////////////////////////
	// Node data access

	//! Gets type of node.
	//! \return Type of node.
	template <>
	node_type xml_node<Ch>::type() const
	{
		return m_type;
	}

	///////////////////////////////////////////////////////////////////////////
	// Related nodes access

	//! Gets document of which node is a child.
	//! \return Pointer to document that contains this node, or 0 if there is no parent document.
	template <>
	xml_document<Ch> *xml_node<Ch>::document() const
	{
		xml_node<Ch> *node = const_cast<xml_node<Ch> *>(this);
		while (node->parent())
			node = node->parent();
		return node->type() == node_document ? static_cast<xml_document<Ch> *>(node) : 0;
	}

	//! Gets first child node, optionally matching node name.
	//! \param name Name of child to find, or 0 to return first child regardless of its name; this string doesn't have to be zero-terminated if name_size is non-zero
	//! \param name_size Size of name, in characters, or 0 to have size calculated automatically from string
	//! \param case_sensitive Should name comparison be case-sensitive; non case-sensitive comparison works properly only for ASCII characters
	//! \return Pointer to found child, or 0 if not found.
	template <>
	xml_node<Ch> *xml_node<Ch>::first_node(const Ch *name, std::size_t name_size, bool case_sensitive) const
	{
		if (name)
		{
			if (name_size == 0)
				name_size = internal::measure(name);
			for (xml_node<Ch> *child = m_first_node; child; child = child->next_sibling())
				if (internal::compare(child->name(), child->name_size(), name, name_size, case_sensitive))
					return child;
			return 0;
		}
		else
			return m_first_node;
	}

	//! Gets last child node, optionally matching node name. 
	//! Behaviour is undefined if node has no children.
	//! Use first_node() to test if node has children.
	//! \param name Name of child to find, or 0 to return last child regardless of its name; this string doesn't have to be zero-terminated if name_size is non-zero
	//! \param name_size Size of name, in characters, or 0 to have size calculated automatically from string
	//! \param case_sensitive Should name comparison be case-sensitive; non case-sensitive comparison works properly only for ASCII characters
	//! \return Pointer to found child, or 0 if not found.
	template <>
	xml_node<Ch> *xml_node<Ch>::last_node(const Ch *name, std::size_t name_size, bool case_sensitive) const
	{
		assert(m_first_node);  // Cannot query for last child if node has no children
		if (name)
		{
			if (name_size == 0)
				name_size = internal::measure(name);
			for (xml_node<Ch> *child = m_last_node; child; child = child->previous_sibling())
				if (internal::compare(child->name(), child->name_size(), name, name_size, case_sensitive))
					return child;
			return 0;
		}
		else
			return m_last_node;
	}

	//! Gets previous sibling node, optionally matching node name. 
	//! Behaviour is undefined if node has no parent.
	//! Use parent() to test if node has a parent.
	//! \param name Name of sibling to find, or 0 to return previous sibling regardless of its name; this string doesn't have to be zero-terminated if name_size is non-zero
	//! \param name_size Size of name, in characters, or 0 to have size calculated automatically from string
	//! \param case_sensitive Should name comparison be case-sensitive; non case-sensitive comparison works properly only for ASCII characters
	//! \return Pointer to found sibling, or 0 if not found.
	template <>
	xml_node<Ch> *xml_node<Ch>::previous_sibling(const Ch *name, std::size_t name_size, bool case_sensitive) const
	{
		assert(this->m_parent);     // Cannot query for siblings if node has no parent
		if (name)
		{
			if (name_size == 0)
				name_size = internal::measure(name);
			for (xml_node<Ch> *sibling = m_prev_sibling; sibling; sibling = sibling->m_prev_sibling)
				if (internal::compare(sibling->name(), sibling->name_size(), name, name_size, case_sensitive))
					return sibling;
			return 0;
		}
		else
			return m_prev_sibling;
	}

	//! Gets next sibling node, optionally matching node name. 
	//! Behaviour is undefined if node has no parent.
	//! Use parent() to test if node has a parent.
	//! \param name Name of sibling to find, or 0 to return next sibling regardless of its name; this string doesn't have to be zero-terminated if name_size is non-zero
	//! \param name_size Size of name, in characters, or 0 to have size calculated automatically from string
	//! \param case_sensitive Should name comparison be case-sensitive; non case-sensitive comparison works properly only for ASCII characters
	//! \return Pointer to found sibling, or 0 if not found.
	template <>
	xml_node<Ch> *xml_node<Ch>::next_sibling(const Ch *name, std::size_t name_size, bool case_sensitive) const
	{
		assert(this->m_parent);     // Cannot query for siblings if node has no parent
		if (name)
		{
			if (name_size == 0)
				name_size = internal::measure(name);
			for (xml_node<Ch> *sibling = m_next_sibling; sibling; sibling = sibling->m_next_sibling)
				if (internal::compare(sibling->name(), sibling->name_size(), name, name_size, case_sensitive))
					return sibling;
			return 0;
		}
		else
			return m_next_sibling;
	}

	//! Gets first attribute of node, optionally matching attribute name.
	//! \param name Name of attribute to find, or 0 to return first attribute regardless of its name; this string doesn't have to be zero-terminated if name_size is non-zero
	//! \param name_size Size of name, in characters, or 0 to have size calculated automatically from string
	//! \param case_sensitive Should name comparison be case-sensitive; non case-sensitive comparison works properly only for ASCII characters
	//! \return Pointer to found attribute, or 0 if not found.
	template <>
	xml_attribute<Ch> *xml_node<Ch>::first_attribute(const Ch *name, std::size_t name_size, bool case_sensitive) const
	{
		if (name)
		{
			if (name_size == 0)
				name_size = internal::measure(name);
			for (xml_attribute<Ch> *attribute = m_first_attribute; attribute; attribute = attribute->m_next_attribute)
				if (internal::compare(attribute->name(), attribute->name_size(), name, name_size, case_sensitive))
					return attribute;
			return 0;
		}
		else
			return m_first_attribute;
	}

	//! Gets last attribute of node, optionally matching attribute name.
	//! \param name Name of attribute to find, or 0 to return last attribute regardless of its name; this string doesn't have to be zero-terminated if name_size is non-zero
	//! \param name_size Size of name, in characters, or 0 to have size calculated automatically from string
	//! \param case_sensitive Should name comparison be case-sensitive; non case-sensitive comparison works properly only for ASCII characters
	//! \return Pointer to found attribute, or 0 if not found.
	template <>
	xml_attribute<Ch> *xml_node<Ch>::last_attribute(const Ch *name, std::size_t name_size, bool case_sensitive) const
	{
		if (name)
		{
			if (name_size == 0)
				name_size = internal::measure(name);
			for (xml_attribute<Ch> *attribute = m_last_attribute; attribute; attribute = attribute->m_prev_attribute)
				if (internal::compare(attribute->name(), attribute->name_size(), name, name_size, case_sensitive))
					return attribute;
			return 0;
		}
		else
			return m_first_attribute ? m_last_attribute : 0;
	}

	///////////////////////////////////////////////////////////////////////////
	// Node modification

	//! Sets type of node.
	//! \param type Type of node to set.
	template <>
	void xml_node<Ch>::type(node_type type)
	{
		m_type = type;
	}

	///////////////////////////////////////////////////////////////////////////
	// Node manipulation

	//! Prepends a new child node.
	//! The prepended child becomes the first child, and all existing children are moved one position back.
	//! \param child Node to prepend.
	template <>
	void xml_node<Ch>::prepend_node(xml_node<Ch> *child)
	{
		assert(child && !child->parent() && child->type() != node_document);
		if (first_node())
		{
			child->m_next_sibling = m_first_node;
			m_first_node->m_prev_sibling = child;
		}
		else
		{
			child->m_next_sibling = 0;
			m_last_node = child;
		}
		m_first_node = child;
		child->m_parent = this;
		child->m_prev_sibling = 0;
	}

	//! Appends a new child node. 
	//! The appended child becomes the last child.
	//! \param child Node to append.
	template <>
	void xml_node<Ch>::append_node(xml_node<Ch> *child)
	{
		assert(child && !child->parent() && child->type() != node_document);
		if (first_node())
		{
			child->m_prev_sibling = m_last_node;
			m_last_node->m_next_sibling = child;
		}
		else
		{
			child->m_prev_sibling = 0;
			m_first_node = child;
		}
		m_last_node = child;
		child->m_parent = this;
		child->m_next_sibling = 0;
	}

	//! Inserts a new child node at specified place inside the node. 
	//! All children after and including the specified node are moved one position back.
	//! \param where Place where to insert the child, or 0 to insert at the back.
	//! \param child Node to insert.
	template <>
	void xml_node<Ch>::insert_node(xml_node<Ch> *where, xml_node<Ch> *child)
	{
		assert(!where || where->parent() == this);
		assert(child && !child->parent() && child->type() != node_document);
		if (where == m_first_node)
			prepend_node(child);
		else if (where == 0)
			append_node(child);
		else
		{
			child->m_prev_sibling = where->m_prev_sibling;
			child->m_next_sibling = where;
			where->m_prev_sibling->m_next_sibling = child;
			where->m_prev_sibling = child;
			child->m_parent = this;
		}
	}

	//! Removes first child node. 
	//! If node has no children, behaviour is undefined.
	//! Use first_node() to test if node has children.
	template <>
	void xml_node<Ch>::remove_first_node()
	{
		assert(first_node());
		xml_node<Ch> *child = m_first_node;
		m_first_node = child->m_next_sibling;
		if (child->m_next_sibling)
			child->m_next_sibling->m_prev_sibling = 0;
		else
			m_last_node = 0;
		child->m_parent = 0;
	}

	//! Removes last child of the node. 
	//! If node has no children, behaviour is undefined.
	//! Use first_node() to test if node has children.
	template <>
	void xml_node<Ch>::remove_last_node()
	{
		assert(first_node());
		xml_node<Ch> *child = m_last_node;
		if (child->m_prev_sibling)
		{
			m_last_node = child->m_prev_sibling;
			child->m_prev_sibling->m_next_sibling = 0;
		}
		else
			m_first_node = 0;
		child->m_parent = 0;
	}

	//! Removes specified child from the node
	// \param where Pointer to child to be removed.
	template <>
	void xml_node<Ch>::remove_node(xml_node<Ch> *where)
	{
		assert(where && where->parent() == this);
		assert(first_node());
		if (where == m_first_node)
			remove_first_node();
		else if (where == m_last_node)
			remove_last_node();
		else
		{
			where->m_prev_sibling->m_next_sibling = where->m_next_sibling;
			where->m_next_sibling->m_prev_sibling = where->m_prev_sibling;
			where->m_parent = 0;
		}
	}


	//! Removes all child nodes (but not attributes).
	template <>
	void xml_node<Ch>::remove_all_nodes()
	{
		for (xml_node<Ch> *node = first_node(); node; node = node->m_next_sibling)
			node->m_parent = 0;
		m_first_node = 0;
	}

	//! Prepends a new attribute to the node.
	//! \param attribute Attribute to prepend.
	template <>
	void xml_node<Ch>::prepend_attribute(xml_attribute<Ch> *attribute)
	{
		assert(attribute && !attribute->parent());
		if (first_attribute())
		{
			attribute->m_next_attribute = m_first_attribute;
			m_first_attribute->m_prev_attribute = attribute;
		}
		else
		{
			attribute->m_next_attribute = 0;
			m_last_attribute = attribute;
		}
		m_first_attribute = attribute;
		attribute->m_parent = this;
		attribute->m_prev_attribute = 0;
	}

	//! Appends a new attribute to the node.
	//! \param attribute Attribute to append.
	template <>
	void xml_node<Ch>::append_attribute(xml_attribute<Ch> *attribute)
	{
		assert(attribute && !attribute->parent());
		if (first_attribute())
		{
			attribute->m_prev_attribute = m_last_attribute;
			m_last_attribute->m_next_attribute = attribute;
		}
		else
		{
			attribute->m_prev_attribute = 0;
			m_first_attribute = attribute;
		}
		m_last_attribute = attribute;
		attribute->m_parent = this;
		attribute->m_next_attribute = 0;
	}

	//! Inserts a new attribute at specified place inside the node. 
	//! All attributes after and including the specified attribute are moved one position back.
	//! \param where Place where to insert the attribute, or 0 to insert at the back.
	//! \param attribute Attribute to insert.
	template <>
	void xml_node<Ch>::insert_attribute(xml_attribute<Ch> *where, xml_attribute<Ch> *attribute)
	{
		assert(!where || where->parent() == this);
		assert(attribute && !attribute->parent());
		if (where == m_first_attribute)
			prepend_attribute(attribute);
		else if (where == 0)
			append_attribute(attribute);
		else
		{
			attribute->m_prev_attribute = where->m_prev_attribute;
			attribute->m_next_attribute = where;
			where->m_prev_attribute->m_next_attribute = attribute;
			where->m_prev_attribute = attribute;
			attribute->m_parent = this;
		}
	}

	//! Removes first attribute of the node. 
	//! If node has no attributes, behaviour is undefined.
	//! Use first_attribute() to test if node has attributes.
	template <>
	void xml_node<Ch>::remove_first_attribute()
	{
		assert(first_attribute());
		xml_attribute<Ch> *attribute = m_first_attribute;
		if (attribute->m_next_attribute)
		{
			attribute->m_next_attribute->m_prev_attribute = 0;
		}
		else
			m_last_attribute = 0;
		attribute->m_parent = 0;
		m_first_attribute = attribute->m_next_attribute;
	}

	//! Removes last attribute of the node. 
	//! If node has no attributes, behaviour is undefined.
	//! Use first_attribute() to test if node has attributes.
	template <>
	void xml_node<Ch>::remove_last_attribute()
	{
		assert(first_attribute());
		xml_attribute<Ch> *attribute = m_last_attribute;
		if (attribute->m_prev_attribute)
		{
			attribute->m_prev_attribute->m_next_attribute = 0;
			m_last_attribute = attribute->m_prev_attribute;
		}
		else
			m_first_attribute = 0;
		attribute->m_parent = 0;
	}

	//! Removes specified attribute from node.
	//! \param where Pointer to attribute to be removed.
	template <>
	void xml_node<Ch>::remove_attribute(xml_attribute<Ch> *where)
	{
		assert(first_attribute() && where->parent() == this);
		if (where == m_first_attribute)
			remove_first_attribute();
		else if (where == m_last_attribute)
			remove_last_attribute();
		else
		{
			where->m_prev_attribute->m_next_attribute = where->m_next_attribute;
			where->m_next_attribute->m_prev_attribute = where->m_prev_attribute;
			where->m_parent = 0;
		}
	}

	//! Removes all attributes of node.
	template <>
	void xml_node<Ch>::remove_all_attributes()
	{
		for (xml_attribute<Ch> *attribute = first_attribute(); attribute; attribute = attribute->m_next_attribute)
			attribute->m_parent = 0;
		m_first_attribute = 0;
	}



	///////////////////////////////////////////////////////////////////////////
	// XML document


	//! Constructs empty XML document
	template <>
	xml_document<Ch>::xml_document()
		: xml_node<Ch>(node_document)
	{
	}
	/*
	template <>
	void xml_document<Ch>::parse(Ch *text)
	{
		assert(text);

		// Remove current contents
		this->remove_all_nodes();
		this->remove_all_attributes();

		// Parse BOM, if any
		parse_bom<Flags>(text);

		// Parse children
		while (1)
		{
			// Skip whitespace before node
			skip<whitespace_pred, Flags>(text);
			if (*text == 0)
				break;

			// Parse and append new child
			if (*text == Ch('<'))
			{
				++text;     // Skip '<'
				if (xml_node<Ch> *node = parse_node<Flags>(text))
					this->append_node(node);
			}
			else
				RAPIDXML_PARSE_ERROR("expected <", text);
		}

	}
	*/

	//! Clears the document by deleting all nodes and clearing the memory pool.
	//! All nodes owned by document pool are destroyed.
	template <>
	void xml_document<Ch>::clear()
	{
		this->remove_all_nodes();
		this->remove_all_attributes();
		memory_pool<Ch>::clear();
	}


};

	/*
private:

	///////////////////////////////////////////////////////////////////////////
	// Restrictions

	// No copying
	xml_node(const xml_node &);
	void operator =(const xml_node &);

	///////////////////////////////////////////////////////////////////////////
	// Data members

	// Note that some of the pointers below have UNDEFINED values if certain other pointers are 0.
	// This is required for maximum performance, as it allows the parser to omit initialization of 
	// unneded/redundant values.
	//
	// The rules are as follows:
	// 1. first_node and first_attribute contain valid pointers, or 0 if node has no children/attributes respectively
	// 2. last_node and last_attribute are valid only if node has at least one child/attribute respectively, otherwise they contain garbage
	// 3. prev_sibling and next_sibling are valid only if node has a parent, otherwise they contain garbage

	node_type m_type;                       // Type of node; always valid
	xml_node<Ch> *m_first_node;             // Pointer to first child node, or 0 if none; always valid
	xml_node<Ch> *m_last_node;              // Pointer to last child node, or 0 if none; this value is only valid if m_first_node is non-zero
	xml_attribute<Ch> *m_first_attribute;   // Pointer to first attribute of node, or 0 if none; always valid
	xml_attribute<Ch> *m_last_attribute;    // Pointer to last attribute of node, or 0 if none; this value is only valid if m_first_attribute is non-zero
	xml_node<Ch> *m_prev_sibling;           // Pointer to previous sibling of node, or 0 if none; this value is only valid if m_parent is non-zero
	xml_node<Ch> *m_next_sibling;           // Pointer to next sibling of node, or 0 if none; this value is only valid if m_parent is non-zero
	*/




