////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Machine Learning Library (EMLL)
//  File:     ElementSelectorNode.tcc (nodes)
//  Authors:  Ofer Dekel
//
////////////////////////////////////////////////////////////////////////////////////////////////////

/// <summary> model namespace </summary>
namespace nodes
{
    template <typename ValueType, typename SelectorType>
    ElementSelectorNode<ValueType, SelectorType>::ElementSelectorNode() : Node({ &_elements, &_selector }, { &_output }), _elements(this, {}, elementsPortName), _selector(this, {}, selectorPortName), _output(this, outputPortName, 1)
    {}
    
    template <typename ValueType, typename SelectorType>
    ElementSelectorNode<ValueType, SelectorType>::ElementSelectorNode(const model::OutputPortElements<ValueType>& input, const model::OutputPortElements<SelectorType>& selector) : Node({ &_elements, &_selector }, { &_output }), _elements(this, input, elementsPortName), _selector(this, selector, selectorPortName), _output(this, outputPortName, 1)
    {
        if (selector.Size() != 1)
        {
            throw std::runtime_error("Error: Condition must be 1-D signal");
        }
    };

    template <typename ValueType, typename SelectorType>
    void ElementSelectorNode<ValueType, SelectorType>::Compute() const
    {
        int index = static_cast<int>(_selector[0]);
        _output.SetOutput({ _elements[index] });
    }

    template <typename ValueType, typename SelectorType>
    void ElementSelectorNode<ValueType, SelectorType>::Serialize(utilities::Serializer& serializer) const
    {
        Node::Serialize(serializer);
        serializer.Serialize("elements", _elements);
        serializer.Serialize("selector", _selector);
        serializer.Serialize("output", _output);
    }

    template <typename ValueType, typename SelectorType>
    void ElementSelectorNode<ValueType, SelectorType>::Deserialize(utilities::Deserializer& serializer, utilities::SerializationContext& context)
    {
        Node::Deserialize(serializer, context);
        serializer.Deserialize("elements", _elements, context);
        serializer.Deserialize("selector", _selector, context);
        serializer.Deserialize("output", _output, context);
    }

    template <typename ValueType, typename SelectorType>
    void ElementSelectorNode<ValueType, SelectorType>::Copy(model::ModelTransformer& transformer) const
    {
        auto newElements = transformer.TransformOutputPortElements(_elements.GetOutputPortElements());
        auto newSelector = transformer.TransformOutputPortElements(_selector.GetOutputPortElements());
        auto newNode = transformer.AddNode<ElementSelectorNode<ValueType, SelectorType>>(newElements, newSelector);
        transformer.MapOutputPort(output, newNode->output);
    }
}
