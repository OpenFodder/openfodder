/*
   Feather INI Parser - 1.41
   You are free to use this however you wish.

   If you find a bug, please attept to debug the cause.
   Post your environment details and the cause or fix in the issues section of GitHub.

   Written by Turbine.

   Website:
   https://github.com/Turbine1991/feather-ini-parser
   http://code.google.com/p/feather-ini-parser/downloads

   Help:
   Bundled example & readme.
   http://code.google.com/p/feather-ini-parser/wiki/Tutorials
*/

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ostream>

#define FINI_SAFE
#define FINI_BUFFER_SIZE 256

#if __cplusplus >= 201103L
#include <unordered_map>
#define FINI_CPP11
#define ALLOCATE_SECTIONS 100
#define ALLOCATE_KEYS 5
#else
#include <map>
#endif

#ifdef FINI_WIDE_SUPPORT
#include <wchar.h>

typedef std::wstringstream fini_sstream_t;
typedef std::wstring fini_string_t;
typedef wchar_t fini_char_t;
typedef std::wifstream fini_ifstream_t;
typedef std::wofstream fini_ofstream_t;

#define fini_strlen(a) wcslen(a)
#define fini_strncpy(a, b) wcscpy(a, b)
#define fini_strncpy(a, b, c) wcsncpy(a, b, c)
#define fini_strtok(a, b) wcstok(a, b)

#define _T(x) L ##x
#else
#include <cstring>

typedef std::stringstream fini_sstream_t;
typedef std::string fini_string_t;
typedef char fini_char_t;
typedef std::ifstream fini_ifstream_t;
typedef std::ofstream fini_ofstream_t;

#define fini_strlen(a) strlen(a)
#define fini_strcpy(a, b) strcpy(a, b)
#define fini_strncpy(a, b, c) strncpy(a, b, c)
#define fini_strtok(a, b) strtok(a, b)

#define _T(x) x
#endif

#define CHAR_SIZE sizeof(fini_char_t)

///Simple converter using templates and streams to effectively required for the flexibility of handling native types
class Converters
{
public:
	template <typename T, typename U>
	static T Convert(U value);
	template <typename T>
	static size_t GetDataSize(T& value);
	static size_t GetDataSize(fini_string_t value);
};

///
template <typename T = fini_string_t, typename U = fini_string_t, typename V = fini_string_t>
class INI
{
public:
	typedef T section_t;
	typedef U key_t;
	typedef V value_t;
	typedef INI<section_t, key_t, value_t> ini_t;

	///Type definition declarations
#ifdef FINI_CPP11
	typedef typename std::unordered_map<key_t, value_t> keys_t;
	typedef typename std::unordered_map<section_t, keys_t*> sections_t;
#else
	typedef typename std::map<key_t, value_t> keys_t;
	typedef typename std::map<section_t, keys_t*> sections_t;
#endif

	typedef typename keys_t::iterator keysit_t;
	typedef typename sections_t::iterator sectionsit_t;

	typedef typename std::pair<key_t, value_t> keyspair_t;
	typedef typename std::pair<section_t, keys_t*> sectionspair_t;

	typedef char data_t;

	enum source_e { SOURCE_FILE, SOURCE_MEMORY };

	///Data members
	std::string filename;
	data_t* data;
	size_t dataSize;
	keys_t* current;
	sections_t sections;
	source_e source;

	///Constuctor/Destructor
	   //Specify the filename to associate and whether to parse immediately
	INI(const std::string filename, bool doParse) : filename(filename)
	{
		init(SOURCE_FILE, doParse);
	}

	//Used for loading INI from memory
	INI(void* data, size_t dataSize, bool doParse) : data((data_t*)data), dataSize(dataSize)
	{
		init(SOURCE_MEMORY, doParse);
	}

	~INI()
	{
		clear();
	}

	///Access Content
	   //Provide bracket access to section contents
	keys_t& operator[](section_t section)
	{
#ifdef FINI_SAFE
		if (!sections[section])
			sections[section] = new keys_t;
#endif

		return *sections[section];
	}

	//Create a new section and select it
	bool create(const section_t section)
	{
		if (select(section))
			return false;

		current = new keys_t;
		sections[section] = current;

		reserveKeys(current);

		return true;
	}

	//Select a section for performing operations
	bool select(const section_t section)
	{
		sectionsit_t sectionsit = sections.find(section);
		if (sectionsit == sections.end())
			return false;

		current = sectionsit->second;

		return true;
	}

	///Debug
	friend std::ostream& operator<<(std::ostream& os, const ini_t& ini)
	{
#ifdef FINI_CPP11
		for (auto i = ini.sections.begin(); i != ini.sections.end(); i++) //typename ini_t::sectionsit_t
		{
			//Section name as ini_t::section_t
			os << '[' << i->first << ']' << std::endl;

			if (i->second->size() == 0)  //No keys/values in section, skip to next
				continue;

			for (typename ini_t::keysit_t j = i->second->begin(); j != i->second->end(); j++)
			{
				//Name as ini_t::key_t & Value as ini_t::key_t
				os << "  " << j->first << "=" << j->second << std::endl;
			}
		}
#else
		std::cout << "Error: FINI requires CPP11 when outputting to stream." << std::endl;
#endif

		return os;
	}

	///Set
	   //Assign a value for key under the selected section
	bool set(const key_t key, const value_t value)
	{
		if (current == NULL)
			return false;

		(*current)[key] = value;

		return true;
	}

	template <typename W, typename X>
	bool set(const W key, const X value)
	{
		return set(Converters::Convert<key_t>(key), Converters::Convert<value_t>(value));
	}

	///Get
	value_t get(const key_t key, value_t def = value_t())
	{
		keysit_t it = current->find(key);
		if (current == NULL || it == current->end())
			return def;

		return it->second;
	}

	value_t get(const section_t section, const key_t key, value_t def)
	{
		if (!select(section))
			return def;

		return get(key, def);
	}

	template <typename W, typename X>
	X get(const W key, const X def = value_t())
	{
		return Converters::Convert<X>(get(Converters::Convert<key_t>(key), Converters::Convert<value_t>(def)));
	}

	template <typename W>
	fini_string_t get(const W key, const fini_char_t* def = _T(""))  //Handle C string default value without casting
	{
		return Converters::Convert<fini_string_t>(get(Converters::Convert<key_t>(key), Converters::Convert<value_t>(def)));
	}

	template <typename W, typename X, typename Y>
	Y get(const W section, const X key, const Y def)
	{
		return Converters::Convert<Y>(get(Converters::Convert<section_t>(section), Converters::Convert<key_t>(key), Converters::Convert<value_t>(def)));
	}

	template <typename W, typename X>
	fini_string_t get(const W section, const X key, const fini_char_t* def)  //Handle C string default value without casting
	{
		return Converters::Convert<fini_string_t>(get(Converters::Convert<section_t>(section), Converters::Convert<key_t>(key), Converters::Convert<value_t>(def)));
	}

	///Functions
	void parse(std::istream& file)
	{
		fini_string_t line;
		bool first = true;
		fini_sstream_t out;
		while (std::getline(file, line))
		{
			if (first)
			{
				first = false;
				if (line[0] == 0xEF) //Allows handling of UTF-16/32 documents
				{
					line.erase(0, 3);
					return;
				}
			}
			if (!line.empty())
			{
				auto len = line.length();
				if (len > 0 && !((len >= 2 && (line[0] == '/' && line[1] == '/')) || (len >= 1 && line[0] == '#')))  //Ignore comment and empty lines
				{
					if (line[0] == ';')
						continue;

					// section
					if (line[0] == '[')
					{
						// without section brackets
						auto ssection = line.substr(1, line.find(']') - 1);
						out << ssection;

						// "convert" to section_t
						section_t section;
						out >> section;

						current = new keys_t;
						sections[section] = current;
					}
					// key
					else
					{
						key_t key;
						value_t value;
						fini_string_t skey = line.substr(0, line.find('='));
						fini_string_t svalue = line.substr(line.find('=') + 1, line.size());
						if (!(skey.empty() || svalue.empty())) {
							skey = skey.substr(skey.find_first_not_of(' '), skey.length());

							// "convert" value
							out << skey;
							out >> key;
							out.clear();
							out.str(fini_string_t());

							// "convert" value
							out << svalue;
							out >> value;

							if (value != value_t())
								(*current)[key] = value;
						}
					}
					out.clear();
					out.str(fini_string_t()); //Clear existing stream;
				}
			}
		}
	}

	//Parse an INI's contents into memory from the filename given during construction
	bool parse()
	{
		switch (source)
		{
		case SOURCE_FILE: {
			fini_ifstream_t file(filename.c_str());

			if (!file.is_open())
				return false;

			parse(file);

			file.close();
		}
						  break;

		case SOURCE_MEMORY: {
			std::stringstream sstream;
			sstream.rdbuf()->pubsetbuf(data, dataSize);

			parse(sstream);
		}
							break;
		}

		return true;
	}

	bool parseBinary()
	{
		fini_ifstream_t file(filename.c_str(), std::ios::binary);
		if (!file.is_open())
			return false;

		size_t sectionCount;
		size_t keyCount;
		key_t key;
		value_t value;
		section_t section;

		//file.read((fini_char_t*)&sectionCount, sizeof(sectionCount));
		file >> sectionCount;

		for (size_t i = 0; i < sectionCount; i++)
		{
			if (i > 0)
				file.seekg(1 + file.tellg());

			file.read((fini_char_t*)&keyCount, sizeof(keyCount));
			file >> section;

			create(section);

			for (size_t j = 0; j < keyCount; j++)
			{
				file >> key;
				file >> value;
				set(key, value);
			}
		}

		file.close();

		return true;
	}

	//Clear the contents from memory
	void clear()
	{
		clean();
		sections.clear();
	}

	///Output
	   //Save from memory into file
	bool save(const std::string filename = "")
	{
		if (!hasFileAssociation(filename))
			return false;

		fini_ofstream_t file(((filename == "") ? this->filename : filename).c_str(), std::ios::trunc);
		if (!file.is_open())
			return false;

		//Loop through sections
		for (typename INI::sectionsit_t i = sections.begin(); i != sections.end(); i++)
		{
			if (i->second->size() == 0)  //No keys/values in section, skip to next
				continue;

			//Write section
			const fini_string_t temp = makeSection(i->first);
			const fini_char_t* line = temp.c_str();
			file.write(line, fini_strlen(line));

			for (typename INI::keysit_t j = i->second->begin(); j != i->second->end(); j++)
			{
				//Write key and value
				const fini_string_t temp = makeKeyValue(j->first, j->second);
				const fini_char_t* line = temp.c_str();
				file.write(line, fini_strlen(line));
			}
		}

		file.close();

		return true;
	}

	//Saves it without any conventional INI formatting characters, however it only uses string streams
	bool saveBinary(const std::string filename = "")
	{
		if (!hasFileAssociation(filename))
			return false;

		fini_ofstream_t file(((filename == "") ? this->filename : filename).c_str(), std::ios::trunc | std::ios::binary);
		if (!file.is_open())
			return false;

		size_t sectionCount = sections.size();
		size_t keyCount;

		file.write((fini_char_t*)&sectionCount, sizeof(sectionCount));

		//Loop through sections
		for (typename INI::sectionsit_t i = sections.begin(); i != sections.end(); i++)
		{
			keyCount = i->second->size();
			file.write((fini_char_t*)&keyCount, sizeof(keyCount));

			file << i->first << std::endl;

			for (typename INI::keysit_t j = i->second->begin(); j != i->second->end(); j++)
			{
				file << j->first << std::endl;
				file << j->second << std::endl;
			}
		}

		file.close();

		return true;
	}

	//Saves it as a true binary file, intended to replace the existing one. Don't bother using it with all strings.
	bool saveBinaryExperimental(std::string filename = "")
	{
		if (!hasFileAssociation(filename))
			return false;

		fini_ofstream_t file(((filename == "") ? this->filename : filename).c_str(), std::ios::trunc | std::ios::binary);
		if (!file.is_open())
			return false;

		size_t sectionCount = sections.size();
		size_t keyCount;

		file.write((fini_char_t*)&sectionCount, sizeof(sectionCount));

		//Loop through sections
		for (typename INI::sectionsit_t i = sections.begin(); i != sections.end(); i++)
		{
			keyCount = i->second->size();
			file.write((fini_char_t*)&keyCount, sizeof(keyCount));

			file.write((fini_char_t*)&i->first, Converters::GetDataSize(i->first));

			for (typename INI::keysit_t j = i->second->begin(); j != i->second->end(); j++)
			{
				file.write((fini_char_t*)&j->first, Converters::GetDataSize(j->first));
				file.write((fini_char_t*)&j->second, Converters::GetDataSize(j->second));
			}
		}

		file.close();

		return true;
	}

	//Alows another INI's contents to be insert into another, with the ability to retain the original values
	void merge(ini_t& other, bool retainValues = true)
	{
		for (typename INI::sectionsit_t i = other.sections.begin(); i != other.sections.end(); i++)
		{
			if (!select(i->first)) //Create and insert all key values into a missing section
			{
				keys_t* keys = new keys_t(*i->second);
				sections.insert(std::make_pair(i->first, keys));
			}
			else
			{
				for (typename INI::keysit_t j = i->second->begin(); j != i->second->end(); j++)
				{
					keysit_t it = current->find(j->first);
					if (it == current->end())
						current->insert(std::make_pair(j->first, j->second));
					else if (!retainValues)
						it->second = j->second;
				}
			}
		}
	}

private:
	///Functions
	   //Init the INI in with values set by constructor
	void init(source_e source, bool doParse)
	{
		this->source = source;

		reserveSections();
		if (doParse)
			parse();
	}

	//Clean the contents for descruction
	void clean()
	{
		for (sectionsit_t i = sections.begin(); i != sections.end(); i++)
			delete i->second;

		current = NULL;
	}

	//Make any alterations to the raw line
	void nake(const fini_char_t*)  //Strip the line of any non-interpretable characters
	{

	}

	void reserveSections()
	{
#ifdef FINI_CPP11
		sections.reserve(ALLOCATE_SECTIONS);
#endif
	}

	void reserveKeys(keys_t* current)
	{
#ifdef FINI_CPP11
		current->reserve(ALLOCATE_KEYS);
#endif
	}

	bool hasFileAssociation(std::string filename)
	{
		if (source == SOURCE_MEMORY && filename == "") //No association to a file
			return false;

		return true;
	}

	///Output
	   //Creates a section as a string
	fini_string_t makeSection(const section_t& section)
	{
		fini_sstream_t line;
		line << '[' << section << ']' << std::endl;

		return line.str();
	}

	//Creates a key and a value as a string
	fini_string_t makeKeyValue(const key_t& key, const value_t& value)
	{
		fini_sstream_t line;
		line << key << '=' << value << std::endl;

		return line.str();
	}
};

///Definitions
template <typename T, typename U>
inline T Converters::Convert(U value)
{
	fini_sstream_t sout;
	T result;

	sout << value;
	sout >> result;

	sout.str(fini_string_t());

	return result;
}

template <>
inline fini_string_t Converters::Convert<fini_string_t, fini_string_t>(fini_string_t value)
{
	return value;
}

template <>
inline fini_string_t Converters::Convert<fini_string_t>(const fini_char_t* value)
{
	return value;
}

template <typename T>
inline size_t Converters::GetDataSize(T& value)
{
	return sizeof(value);
}

inline size_t Converters::GetDataSize(fini_string_t value)
{
	return value.size() + 1;
}
