add_library(entitymapping OBJECT
        Actions/Action.cpp Actions/DummyAction.cpp

        Cases/AttributeCase.cpp
        Cases/AttributeComparers/HeightComparerWrapper.cpp
        Cases/AttributeComparers/NumericComparerWrapper.cpp Cases/AttributeComparers/NumericEqualsComparer.cpp
        Cases/AttributeComparers/NumericEqualsOrGreaterComparer.cpp Cases/AttributeComparers/NumericEqualsOrLesserComparer.cpp
        Cases/AttributeComparers/NumericGreaterComparer.cpp Cases/AttributeComparers/NumericLesserComparer.cpp
        Cases/AttributeComparers/NumericRangeComparer.cpp Cases/AttributeComparers/NumericValueHolder.cpp
        Cases/AttributeComparers/StringComparer.cpp Cases/AttributeComparers/StringComparerWrapper.cpp
        Cases/CaseBase.cpp Cases/EntityBaseCase.cpp Cases/EntityTypeCase.cpp Cases/EntityRefCase.cpp

        ChangeContext.cpp

        Matches/AttributeMatch.cpp Matches/EntityTypeMatch.cpp
        Matches/Observers/MatchAttributeObserver.cpp Matches/Observers/EntityCreationObserver.cpp Matches/EntityRefMatch.cpp
        EntityMapping.cpp EntityMappingCreator.cpp EntityMappingManager.cpp
        IActionCreator.h IVisitor.h Matches/VirtualAttributeMatch.cpp Matches/VirtualAttributeMatch.h Matches/SingleAttributeMatch.cpp Matches/SingleAttributeMatch.h)


