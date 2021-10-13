// COPYRIGHT 1995-2021 ESRI
// TRADE SECRETS: ESRI PROPRIETARY AND CONFIDENTIAL
// Unpublished material - all rights reserved under the
// Copyright Laws of the United States and applicable international
// laws, treaties, and conventions.
//
// For additional information, contact:
// Environmental Systems Research Institute, Inc.
// Attn: Contracts and Legal Services Department
// 380 New York Street
// Redlands, California, 92373
// USA
//
// email: contracts@esri.com

#include "ArcGISMapsSDK/API/ErrorManager.h"
#include "ArcGISMapsSDK/API/GameEngine/Attributes/Attribute.h"
#include "ArcGISMapsSDK/API/GameEngine/Attributes/VisualizationAttribute.h"
#include "ArcGISMapsSDK/API/GameEngine/Attributes/VisualizationAttributeDescription.h"
#include "ArcGISMapsSDK/API/GameEngine/Security/ArcGISAuthenticationConfiguration.h"
#include "ArcGISMapsSDK/API/Standard/Element.h" // IWYU pragma: associated
#include "ArcGISMapsSDK/API/Standard/ElementType.h"
#include "ArcGISMapsSDK/API/Standard/Error.h"
#include "ArcGISMapsSDK/API/Standard/ErrorHandler.h"

namespace Esri
{
namespace Standard
{
Element::Element()
{
	auto errorHandler = ErrorManager::CreateHandler();

	m_handle = RT_Element_create(reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));
}

ElementType Element::GetObjectType() const
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult =
		RT_Element_getObjectType(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	return static_cast<ElementType>(localResult);
}

void Element::Clear()
{
	auto errorHandler = ErrorManager::CreateHandler();

	RT_Element_clear(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));
}

bool Element::Equals(const Element& element2) const
{
	auto errorHandler = ErrorManager::CreateHandler();

	const auto localElement2 = static_cast<const RT_ElementHandle>(element2.GetHandle());

	auto localResult = RT_Element_equals(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), localElement2,
										 reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	return localResult;
}

Element Element::FromArcGISAuthenticationConfiguration(const GameEngine::Security::ArcGISAuthenticationConfiguration& value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	const auto localArcGISAuthenticationConfiguration = static_cast<const RT_ArcGISAuthenticationConfigurationHandle>(value.GetHandle());

	auto localResult = RT_Element_fromArcGISAuthenticationConfiguration(localArcGISAuthenticationConfiguration,
																		reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	auto localLocalResult = Element{};

	if (localResult != nullptr)
	{
		localLocalResult = Element{localResult};
	}

	return localLocalResult;
}

Element Element::FromBool(bool value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult = RT_Element_fromBool(value, reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	auto localLocalResult = Element{};

	if (localResult != nullptr)
	{
		localLocalResult = Element{localResult};
	}

	return localLocalResult;
}

Element Element::FromFloat32(float value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult = RT_Element_fromFloat32(value, reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	auto localLocalResult = Element{};

	if (localResult != nullptr)
	{
		localLocalResult = Element{localResult};
	}

	return localLocalResult;
}

Element Element::FromFloat64(double value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult = RT_Element_fromFloat64(value, reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	auto localLocalResult = Element{};

	if (localResult != nullptr)
	{
		localLocalResult = Element{localResult};
	}

	return localLocalResult;
}

Element Element::FromInt16(int16_t value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult = RT_Element_fromInt16(value, reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	auto localLocalResult = Element{};

	if (localResult != nullptr)
	{
		localLocalResult = Element{localResult};
	}

	return localLocalResult;
}

Element Element::FromInt32(int32_t value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult = RT_Element_fromInt32(value, reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	auto localLocalResult = Element{};

	if (localResult != nullptr)
	{
		localLocalResult = Element{localResult};
	}

	return localLocalResult;
}

Element Element::FromInt64(int64_t value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult = RT_Element_fromInt64(value, reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	auto localLocalResult = Element{};

	if (localResult != nullptr)
	{
		localLocalResult = Element{localResult};
	}

	return localLocalResult;
}

Element Element::FromInt8(int8_t value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult = RT_Element_fromInt8(value, reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	auto localLocalResult = Element{};

	if (localResult != nullptr)
	{
		localLocalResult = Element{localResult};
	}

	return localLocalResult;
}

Element Element::FromString(const FString& value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	const auto* localValue = TCHAR_TO_ANSI(*value);

	auto localResult = RT_Element_fromString(localValue, reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	auto localLocalResult = Element{};

	if (localResult != nullptr)
	{
		localLocalResult = Element{localResult};
	}

	return localLocalResult;
}

Element Element::FromUInt16(uint16_t value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult = RT_Element_fromUInt16(value, reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	auto localLocalResult = Element{};

	if (localResult != nullptr)
	{
		localLocalResult = Element{localResult};
	}

	return localLocalResult;
}

Element Element::FromUInt32(uint32_t value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult = RT_Element_fromUInt32(value, reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	auto localLocalResult = Element{};

	if (localResult != nullptr)
	{
		localLocalResult = Element{localResult};
	}

	return localLocalResult;
}

Element Element::FromUInt64(uint64_t value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult = RT_Element_fromUInt64(value, reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	auto localLocalResult = Element{};

	if (localResult != nullptr)
	{
		localLocalResult = Element{localResult};
	}

	return localLocalResult;
}

Element Element::FromUInt8(uint8_t value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult = RT_Element_fromUInt8(value, reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	auto localLocalResult = Element{};

	if (localResult != nullptr)
	{
		localLocalResult = Element{localResult};
	}

	return localLocalResult;
}

Element Element::FromVisualizationAttributeDescription(const GameEngine::Attributes::VisualizationAttributeDescription& value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	const auto localVisualizationAttributeDescription = static_cast<const RT_VisualizationAttributeDescriptionHandle>(value.GetHandle());

	auto localResult = RT_Element_fromVisualizationAttributeDescription(localVisualizationAttributeDescription,
																		reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	auto localLocalResult = Element{};

	if (localResult != nullptr)
	{
		localLocalResult = Element{localResult};
	}

	return localLocalResult;
}

GameEngine::Attributes::Attribute Element::GetValueAsAttribute() const
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult =
		RT_Element_getValueAsAttribute(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	auto localLocalResult = GameEngine::Attributes::Attribute{};

	if (localResult != nullptr)
	{
		localLocalResult = GameEngine::Attributes::Attribute{localResult};
	}

	return localLocalResult;
}

bool Element::GetValueAsBool() const
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult =
		RT_Element_getValueAsBool(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	return localResult;
}

Error Element::GetValueAsError() const
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult =
		RT_Element_getValueAsError(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	auto localLocalResult = Error{};

	if (localResult != nullptr)
	{
		localLocalResult = Error{localResult};
	}

	return localLocalResult;
}

float Element::GetValueAsFloat32() const
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult =
		RT_Element_getValueAsFloat32(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	return localResult;
}

double Element::GetValueAsFloat64() const
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult =
		RT_Element_getValueAsFloat64(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	return localResult;
}

int16_t Element::GetValueAsInt16() const
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult =
		RT_Element_getValueAsInt16(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	return localResult;
}

int32_t Element::GetValueAsInt32() const
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult =
		RT_Element_getValueAsInt32(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	return localResult;
}

int64_t Element::GetValueAsInt64() const
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult =
		RT_Element_getValueAsInt64(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	return localResult;
}

int8_t Element::GetValueAsInt8() const
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult =
		RT_Element_getValueAsInt8(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	return localResult;
}

FString Element::GetValueAsString() const
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult =
		RT_Element_getValueAsString(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	auto localLocalResult = FString{};

	if (localResult != nullptr)
	{
		localLocalResult = FString{RT_String_cStr(localResult, reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()))};
		RT_String_destroy(localResult, reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));
	}

	return localLocalResult;
}

uint16_t Element::GetValueAsUInt16() const
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult =
		RT_Element_getValueAsUInt16(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	return localResult;
}

uint32_t Element::GetValueAsUInt32() const
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult =
		RT_Element_getValueAsUInt32(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	return localResult;
}

uint64_t Element::GetValueAsUInt64() const
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult =
		RT_Element_getValueAsUInt64(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	return localResult;
}

uint8_t Element::GetValueAsUInt8() const
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult =
		RT_Element_getValueAsUInt8(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	return localResult;
}

GameEngine::Attributes::VisualizationAttribute Element::GetValueAsVisualizationAttribute() const
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult = RT_Element_getValueAsVisualizationAttribute(reinterpret_cast<RT_ElementHandle>(this->GetHandle()),
																   reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	auto localLocalResult = GameEngine::Attributes::VisualizationAttribute{};

	if (localResult != nullptr)
	{
		localLocalResult = GameEngine::Attributes::VisualizationAttribute{localResult};
	}

	return localLocalResult;
}

GameEngine::Attributes::VisualizationAttributeDescription Element::GetValueAsVisualizationAttributeDescription() const
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult = RT_Element_getValueAsVisualizationAttributeDescription(reinterpret_cast<RT_ElementHandle>(this->GetHandle()),
																			  reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	auto localLocalResult = GameEngine::Attributes::VisualizationAttributeDescription{};

	if (localResult != nullptr)
	{
		localLocalResult = GameEngine::Attributes::VisualizationAttributeDescription{localResult};
	}

	return localLocalResult;
}

bool Element::IsEmpty() const
{
	auto errorHandler = ErrorManager::CreateHandler();

	auto localResult =
		RT_Element_isEmpty(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));

	return localResult;
}

void Element::SetValueFromBool(bool value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	RT_Element_setValueFromBool(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), value, reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));
}

void Element::SetValueFromFloat32(float value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	RT_Element_setValueFromFloat32(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), value,
								   reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));
}

void Element::SetValueFromFloat64(double value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	RT_Element_setValueFromFloat64(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), value,
								   reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));
}

void Element::SetValueFromInt16(int16_t value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	RT_Element_setValueFromInt16(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), value,
								 reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));
}

void Element::SetValueFromInt32(int32_t value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	RT_Element_setValueFromInt32(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), value,
								 reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));
}

void Element::SetValueFromInt64(int64_t value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	RT_Element_setValueFromInt64(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), value,
								 reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));
}

void Element::SetValueFromInt8(int8_t value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	RT_Element_setValueFromInt8(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), value, reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));
}

void Element::SetValueFromString(const FString& value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	const auto* localValue = TCHAR_TO_ANSI(*value);

	RT_Element_setValueFromString(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), localValue,
								  reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));
}

void Element::SetValueFromUInt16(uint16_t value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	RT_Element_setValueFromUInt16(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), value,
								  reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));
}

void Element::SetValueFromUInt32(uint32_t value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	RT_Element_setValueFromUInt32(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), value,
								  reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));
}

void Element::SetValueFromUInt64(uint64_t value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	RT_Element_setValueFromUInt64(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), value,
								  reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));
}

void Element::SetValueFromUInt8(uint8_t value)
{
	auto errorHandler = ErrorManager::CreateHandler();

	RT_Element_setValueFromUInt8(reinterpret_cast<RT_ElementHandle>(this->GetHandle()), value,
								 reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

	ErrorManager::CheckError(std::move(errorHandler));
}

Element::Element(void* handle) : m_handle{handle}
{
}

Element::Element(Element&& other) noexcept : m_handle{other.m_handle}
{
	other.m_handle = nullptr;
}

Element::~Element()
{
	if (this->m_handle)
	{
		auto errorHandler = ErrorManager::CreateHandler();

		RT_Element_destroy(reinterpret_cast<RT_ElementHandle>(m_handle), reinterpret_cast<RT_ErrorHandler*>(errorHandler.get()));

		ErrorManager::CheckError(std::move(errorHandler));
	}
}

Element& Element::operator=(Element&& other) noexcept
{
	if (this != &other)
	{
		using std::swap;

		swap(m_handle, other.m_handle);
	}

	return *this;
}

void* Element::GetHandle() const
{
	return m_handle;
}
} // namespace Standard
} // namespace Esri
