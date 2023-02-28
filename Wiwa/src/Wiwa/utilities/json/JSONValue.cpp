#include "wipch.h"

#include "JSONValue.h"

namespace Wiwa
{
	JSONValue::JSONValue(rapidjson::Value *value, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> *allocator)
	{
		m_Value = value;
		m_Allocator = allocator;
	}

	JSONValue::~JSONValue()
	{
	}

	JSONValue JSONValue::AddMemberObject(const char *mem)
	{
		rapidjson::Value key(mem, *m_Allocator);
		rapidjson::Value v(rapidjson::kObjectType);

		m_Value->AddMember(key, v, *m_Allocator);

		rapidjson::Value &jval = m_Value->operator[](mem);

		return JSONValue(&jval, m_Allocator);
	}

	JSONValue JSONValue::AddMemberArray(const char *mem)
	{
		rapidjson::Value key(mem, *m_Allocator);
		rapidjson::Value v(rapidjson::kArrayType);

		m_Value->AddMember(key, v, *m_Allocator);

		rapidjson::Value &jval = m_Value->operator[](mem);

		return JSONValue(&jval, m_Allocator);
	}

	JSONValue JSONValue::Begin()
	{
		rapidjson::Value *jval = m_Value->Begin();

		return JSONValue(jval, m_Allocator);
	}

	JSONValue JSONValue::End()
	{
		rapidjson::Value *jval = m_Value->End();

		return JSONValue(jval, m_Allocator);
	}

	JSONValue JSONValue::Next()
	{
		rapidjson::Value *jval = m_Value->Begin();
		jval++;

		return JSONValue(jval, m_Allocator);
	}

	size_t JSONValue::Size()
	{
		return m_Value->Size();
	}

	bool JSONValue::IsObject()
	{
		return m_Value->IsObject();
	}

	bool JSONValue::IsArray()
	{
		return m_Value->IsArray();
	}

	JSONValue JSONValue::PushBackObject()
	{
		rapidjson::Value obj(rapidjson::kObjectType);

		m_Value->PushBack(obj, *m_Allocator);

		rapidjson::Value *jval = m_Value->End();
		jval--;

		return {jval, m_Allocator};
	}

	bool JSONValue::HasMember(const char *mem)
	{
		return m_Value->HasMember(mem);
	}

	JSONValue JSONValue::operator[](const char *key)
	{
		return JSONValue(&m_Value->operator[](key), m_Allocator);
	}

	JSONValue JSONValue::operator[](uint32_t index)
	{
		return JSONValue(&m_Value->operator[](index), m_Allocator);
	}
}
