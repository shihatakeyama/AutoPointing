// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// rapidxml.h
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


#ifndef RAPIDXML_H
#define RAPIDXML_H


//#include "rapidxml.hpp"


namespace rapidxml{


	///////////////////////////////////////////////////////////////////////////
	// XML base
	template<class Ch = char>
	class xml_base
	{

	public:
		xml_base();

		Ch *name() const;

		std::size_t name_size() const;

		Ch *value() const;

		std::size_t value_size() const;

		void name(const Ch *name, std::size_t size);

		void name(const Ch *name);

		void value(const Ch *value, std::size_t size);

		void value(const Ch *value);

		xml_node<Ch> *parent() const;

	protected:

		// Return empty string
		static Ch *nullstr();

		Ch *m_name;                         // Name of node, or 0 if no name
		Ch *m_value;                        // Value of node, or 0 if no value
		std::size_t m_name_size;            // Length of node name, or undefined of no name
		std::size_t m_value_size;           // Length of node value, or undefined if no value
		xml_node<Ch> *m_parent;             // Pointer to parent node, or 0 if none

	};


	///////////////////////////////////////////////////////////////////////////
	// xml_attribute

	template<class Ch = char>
	class xml_attribute : public xml_base<Ch>
	{

		friend class xml_node<Ch>;

	public:

		xml_attribute();

		xml_document<Ch> *document() const;

		xml_attribute<Ch> *previous_attribute(const Ch *name = 0, std::size_t name_size = 0, bool case_sensitive = true) const;

		xml_attribute<Ch> *next_attribute(const Ch *name = 0, std::size_t name_size = 0, bool case_sensitive = true) const;

	private:

		xml_attribute<Ch> *m_prev_attribute;        // Pointer to previous sibling of attribute, or 0 if none; only valid if parent is non-zero
		xml_attribute<Ch> *m_next_attribute;        // Pointer to next sibling of attribute, or 0 if none; only valid if parent is non-zero

	};


	///////////////////////////////////////////////////////////////////////////
	// xml_node
	
	template<class Ch = char>
	class xml_node : public xml_base < Ch >
	{

	public:

		xml_node(node_type type);

		node_type type() const;

		xml_document<Ch> *document() const;

		xml_node<Ch> *first_node(const Ch *name = 0, std::size_t name_size = 0, bool case_sensitive = true) const;

		xml_node<Ch> *last_node(const Ch *name = 0, std::size_t name_size = 0, bool case_sensitive = true) const;

		xml_node<Ch> *previous_sibling(const Ch *name = 0, std::size_t name_size = 0, bool case_sensitive = true) const;

		xml_node<Ch> *next_sibling(const Ch *name = 0, std::size_t name_size = 0, bool case_sensitive = true) const;

		xml_attribute<Ch> *first_attribute(const Ch *name = 0, std::size_t name_size = 0, bool case_sensitive = true) const;

		xml_attribute<Ch> *last_attribute(const Ch *name = 0, std::size_t name_size = 0, bool case_sensitive = true) const;

		void type(node_type type);

		void prepend_node(xml_node<Ch> *child);

		void append_node(xml_node<Ch> *child);

		void insert_node(xml_node<Ch> *where, xml_node<Ch> *child);

		void remove_first_node();

		void remove_last_node();

		void remove_node(xml_node<Ch> *where);

		//! Removes all child nodes (but not attributes).
		void remove_all_nodes();

		//! Prepends a new attribute to the node.
		//! \param attribute Attribute to prepend.
		void prepend_attribute(xml_attribute<Ch> *attribute);

		//! Appends a new attribute to the node.
		//! \param attribute Attribute to append.
		void append_attribute(xml_attribute<Ch> *attribute);

		//! Inserts a new attribute at specified place inside the node. 
		//! All attributes after and including the specified attribute are moved one position back.
		//! \param where Place where to insert the attribute, or 0 to insert at the back.
		//! \param attribute Attribute to insert.
		void insert_attribute(xml_attribute<Ch> *where, xml_attribute<Ch> *attribute);

		void remove_first_attribute();

		void remove_last_attribute();

		void remove_attribute(xml_attribute<Ch> *where);

		//! Removes all attributes of node.
		void remove_all_attributes();

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

	};

};

#endif RAPIDXML_H



