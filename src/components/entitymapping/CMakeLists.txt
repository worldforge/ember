add_library(entitymapping
        Actions/Action.cpp Actions/DummyAction.cpp
        Cases/AttributeCase.cpp Cases/AttributeComparers/AttributeComparerWrapper.cpp
        Cases/AttributeComparers/HeightComparerWrapper.cpp Cases/AttributeComparers/NumericComparer.cpp
        Cases/AttributeComparers/NumericComparerWrapper.cpp Cases/AttributeComparers/NumericEqualsComparer.cpp
        Cases/AttributeComparers/NumericEqualsOrGreaterComparer.cpp Cases/AttributeComparers/NumericEqualsOrLesserComparer.cpp
        Cases/AttributeComparers/NumericGreaterComparer.cpp Cases/AttributeComparers/NumericLesserComparer.cpp
        Cases/AttributeComparers/NumericRangeComparer.cpp Cases/AttributeComparers/NumericValueHolder.cpp
        Cases/AttributeComparers/StringComparer.cpp Cases/AttributeComparers/StringComparerWrapper.cpp Cases/Case.cpp
        Cases/CaseBase.cpp Cases/EntityBaseCase.cpp Cases/EntityTypeCase.cpp Cases/OutfitCase.cpp
        ChangeContext.cpp Definitions/ActionDefinition.cpp Definitions/CaseDefinition.cpp
        Definitions/DefinitionBase.cpp Definitions/MatchDefinition.cpp Definitions/EntityMappingDefinition.cpp
        Matches/AbstractMatch.cpp Matches/AttributeDependentMatch.cpp
        Matches/AttributeMatch.cpp Matches/EntityTypeMatch.cpp Matches/MatchBase.cpp
        Matches/Observers/MatchAttributeObserver.cpp Matches/Observers/EntityCreationObserver.cpp Matches/OutfitMatch.cpp
        EntityMapping.cpp EntityMappingCreator.cpp EntityMappingManager.cpp
        IActionCreator.h IVisitor.h)


