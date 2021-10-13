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

#pragma once

namespace Esri
{
namespace GameEngine
{
namespace Attributes
{
class Attribute;
class VisualizationAttribute;
class VisualizationAttributeDescription;
} // namespace Attributes

namespace Security
{
class ArcGISAuthenticationConfiguration;
} // namespace Security
} // namespace GameEngine

namespace Standard
{
enum class ElementType;
class Error;

class Element
{
public:
#pragma region Constructors
	/// Creates an empty, unknown element type.
	///
	/// - Since: 100.0.0
	Element();
#pragma endregion Constructors

#pragma region Properties
	/// The type that the element is holding.
	///
	/// - Since: 100.0.0
	ElementType GetObjectType() const;
#pragma endregion Properties

#pragma region Methods
	/// Removes the value from the element and sets the type to ElementType.none.
	///
	/// - Since: 100.0.0
	void Clear();

	/// Tests if two element are equal.
	///
	/// - Parameter element2: The second element.
	/// - Returns: true if the two elements are equal, false otherwise.
	/// - Since: 100.0.0
	bool Equals(const Element& element2) const;

	/// Creates an element object from an authentication configuration.
	///
	/// - Parameter value: The value.
	/// - Returns: A Element.
	/// - Since: 100.11.0
	static Element FromArcGISAuthenticationConfiguration(const GameEngine::Security::ArcGISAuthenticationConfiguration& value);

	/// Creates an element from an boolean value.
	///
	/// - Parameter value: The value.
	/// - Returns: A Element.
	/// - Since: 100.0.0
	static Element FromBool(bool value);

	/// Creates an element from a float32 value.
	///
	/// - Parameter value: The value.
	/// - Returns: A Element.
	/// - Since: 100.0.0
	static Element FromFloat32(float value);

	/// Creates an element from a float64 value.
	///
	/// - Parameter value: The value.
	/// - Returns: A Element.
	/// - Since: 100.0.0
	static Element FromFloat64(double value);

	/// Creates an element from an int16_t value.
	///
	/// - Parameter value: The value.
	/// - Returns: A Element.
	/// - Since: 100.0.0
	static Element FromInt16(int16_t value);

	/// Creates an element from an int32_t value.
	///
	/// - Parameter value: The value.
	/// - Returns: A Element.
	/// - Since: 100.0.0
	static Element FromInt32(int32_t value);

	/// Creates an element from an int64_t value.
	///
	/// - Parameter value: The value.
	/// - Returns: A Element
	/// - Since: 100.0.0
	static Element FromInt64(int64_t value);

	/// Creates an element from an int8_t value.
	///
	/// - Parameter value: The value.
	/// - Returns: A Element.
	/// - Since: 100.0.0
	static Element FromInt8(int8_t value);

	/// Creates an element from a string value.
	///
	/// - Parameter value: The value.
	/// - Returns: A Element.
	/// - Since: 100.0.0
	static Element FromString(const FString& value);

	/// Creates an element from a uint16_t value.
	///
	/// - Parameter value: The value.
	/// - Returns: A Element.
	/// - Since: 100.0.0
	static Element FromUInt16(uint16_t value);

	/// Creates an element from a uint32_t value.
	///
	/// - Parameter value: The value.
	/// - Returns: A Element.
	/// - Since: 100.0.0
	static Element FromUInt32(uint32_t value);

	/// Creates an element from a uint64_t value.
	///
	/// - Parameter value: The value.
	/// - Returns: A Element.
	/// - Since: 100.0.0
	static Element FromUInt64(uint64_t value);

	/// Creates an element from a uint8_t value.
	///
	/// - Parameter value: The value.
	/// - Returns: A Element
	/// - Since: 100.0.0
	static Element FromUInt8(uint8_t value);

	/// Creates an element object from a visualization attribute description.
	///
	/// - Parameter value: The value.
	/// - Returns: A Element
	/// - Since: 100.12.0
	static Element FromVisualizationAttributeDescription(const GameEngine::Attributes::VisualizationAttributeDescription& value);

	/// Gets the value of an element as an attribute.
	///
	/// - Returns: A Attribute.
	/// - Since: 100.12.0
	GameEngine::Attributes::Attribute GetValueAsAttribute() const;

	/// Gets the value of the element.
	///
	/// - Returns: An boolean. Returns false if an error occurs.
	/// - Since: 100.0.0
	bool GetValueAsBool() const;

	/// Gets the value of the element.
	///
	/// - Returns: An Error.
	/// - Since: 100.1.0
	Error GetValueAsError() const;

	/// Gets the value of the element.
	///
	/// - Returns: An float32.
	/// - Since: 100.0.0
	float GetValueAsFloat32() const;

	/// Gets the value of the element.
	///
	/// - Returns: An float64.
	/// - Since: 100.0.0
	double GetValueAsFloat64() const;

	/// Gets the value of the element.
	///
	/// - Returns: An int16_t.
	/// - Since: 100.0.0
	int16_t GetValueAsInt16() const;

	/// Gets the value of the element.
	///
	/// - Returns: An int32_t.
	/// - Since: 100.0.0
	int32_t GetValueAsInt32() const;

	/// Gets the value of the element.
	///
	/// - Returns: An int64_t.
	/// - Since: 100.0.0
	int64_t GetValueAsInt64() const;

	/// Gets the value of the element.
	///
	/// - Returns: An int8_t.
	/// - Since: 100.0.0
	int8_t GetValueAsInt8() const;

	/// Gets the value of the element.
	///
	/// - Returns: An String.
	/// - Since: 100.0.0
	FString GetValueAsString() const;

	/// Gets the value of the element.
	///
	/// - Returns: An uint16_t.
	/// - Since: 100.0.0
	uint16_t GetValueAsUInt16() const;

	/// Gets the value of the element.
	///
	/// - Returns: An uint32_t.
	/// - Since: 100.0.0
	uint32_t GetValueAsUInt32() const;

	/// Gets the value of the element.
	///
	/// - Returns: An uint64_t.
	/// - Since: 100.0.0
	uint64_t GetValueAsUInt64() const;

	/// Gets the value of the element.
	///
	/// - Returns: An uint8_t.
	/// - Since: 100.0.0
	uint8_t GetValueAsUInt8() const;

	/// Gets the value of an element as a visualization attribute.
	///
	/// - Returns: A VisualizationAttribute.
	/// - Since: 100.12.0
	GameEngine::Attributes::VisualizationAttribute GetValueAsVisualizationAttribute() const;

	/// Gets the value of an element as a visualization attribute description.
	///
	/// - Returns: A VisualizationAttributeDescription.
	/// - Since: 100.12.0
	GameEngine::Attributes::VisualizationAttributeDescription GetValueAsVisualizationAttributeDescription() const;

	/// Returns true if the element doesn't have a value.
	///
	/// - Returns: true if the element doesn't have a value, false otherwise.
	/// - Since: 100.0.0
	bool IsEmpty() const;

	/// Sets the value of the element.
	///
	/// - Parameter value: The value.
	/// - Since: 100.0.0
	void SetValueFromBool(bool value);

	/// Sets the value of the element.
	///
	/// - Parameter value: The value.
	/// - Since: 100.0.0
	void SetValueFromFloat32(float value);

	/// Sets the value of the element.
	///
	/// - Parameter value: The value.
	/// - Since: 100.0.0
	void SetValueFromFloat64(double value);

	/// Sets the value of the element.
	///
	/// - Parameter value: The value.
	/// - Since: 100.0.0
	void SetValueFromInt16(int16_t value);

	/// Sets the value of the element.
	///
	/// - Parameter value: The value.
	/// - Since: 100.0.0
	void SetValueFromInt32(int32_t value);

	/// Sets the value of the element.
	///
	/// - Parameter value: The value.
	/// - Since: 100.0.0
	void SetValueFromInt64(int64_t value);

	/// Sets the value of the element.
	///
	/// - Parameter value: The value.
	/// - Since: 100.0.0
	void SetValueFromInt8(int8_t value);

	/// Sets the value of the element.
	///
	/// - Parameter value: The value.
	/// - Since: 100.0.0
	void SetValueFromString(const FString& value);

	/// Sets the value of the element.
	///
	/// - Parameter value: The value.
	/// - Since: 100.0.0
	void SetValueFromUInt16(uint16_t value);

	/// Sets the value of the element.
	///
	/// - Parameter value: The value.
	/// - Since: 100.0.0
	void SetValueFromUInt32(uint32_t value);

	/// Sets the value of the element.
	///
	/// - Parameter value: The value.
	/// - Since: 100.0.0
	void SetValueFromUInt64(uint64_t value);

	/// Sets the value of the element.
	///
	/// - Parameter value: The value.
	/// - Since: 100.0.0
	void SetValueFromUInt8(uint8_t value);
#pragma endregion Methods

public:
#pragma region Special Members
	explicit Element(void* handle);
	Element(const Element&) = delete;
	Element(Element&& other) noexcept;
	~Element();

	Element& operator=(const Element&) = delete;
	Element& operator=(Element&& other) noexcept;

	void* GetHandle() const;
#pragma endregion Special Members

protected:
	void* m_handle{nullptr};
};
} // namespace Standard
} // namespace Esri
