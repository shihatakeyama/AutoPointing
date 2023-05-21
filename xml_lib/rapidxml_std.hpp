// **********************************************************************
//  rapidxml_std.hpp
//  
//
// **********************************************************************

#ifndef RAPID_XML_STD_HPP
#define RAPID_XML_STD_HPP

template<typename Ch = rapidxml::char_t>
class children {
  typedef rapidxml::node_iterator<Ch> iterator_type;
public:
  explicit children(rapidxml::xml_node<Ch>* node) : node_(node) {}
  iterator_type begin() const { return iterator_type(node_); }
  iterator_type end() const { return iterator_type(); }
private:
  rapidxml::xml_node<Ch>* node_;
};

template<typename Ch = rapidxml::char_t>
class attributes {
  typedef rapidxml::attribute_iterator<Ch> iterator_type;
public:
  explicit attributes(rapidxml::xml_node<Ch>* node) : node_(node) {}
  iterator_type begin() const { return iterator_type(node_); }
  iterator_type end() const { return iterator_type(); }
private:
  rapidxml::xml_node<Ch>* node_;
};


namespace std {
	template<typename Ch> 
	inline rapidxml::node_iterator<Ch> begin(rapidxml::node_iterator<Ch> iter){
		return iter; 
	};

	template<typename Ch>
	inline rapidxml::node_iterator<Ch> end(rapidxml::node_iterator<Ch> iter){
		return rapidxml::node_iterator<Ch>();
	}


	template<typename Ch>
	inline rapidxml::attribute_iterator<Ch> 
    begin(rapidxml::attribute_iterator<Ch> iter){ 
		return iter; 
	};

	template<typename Ch>
	inline rapidxml::attribute_iterator<Ch> end(rapidxml::attribute_iterator<Ch> iter){
		return rapidxml::attribute_iterator<Ch>(); 
	};
}

#endif // RAPID_XML_STD_HPP
