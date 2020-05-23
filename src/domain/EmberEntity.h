/*
 Copyright (C) 2004  Erik Ogenvik <erik@ogenvik.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef EMBEROGRE_EMBERENTITY_H
#define EMBEROGRE_EMBERENTITY_H

#include "IVisualizable.h"

#include <Atlas/Objects/Entity.h>
#include <Atlas/Objects/Operation.h>

#include <Eris/ViewEntity.h>
#include <Eris/Usage.h>

#include <functional>

namespace Eris
{
class View;
}

namespace Ember
{
struct IGraphicalRepresentation;
struct IEntityAttachment;
struct IEntityControlDelegate;
struct IEntityVisitor;
struct IHeightProvider;
class EntityTalk;


/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief A representation of an Eris::Entity, ie. a world entity.
 *
 * This is the base class for any entities in the world. It's therefore safe to case any instance of Eris::Entity into this class.
 * Any entity which has a graphical representation in the world, and under normal circumstances that's the most of them, will have mGraphicalRepresentation set to a subclass of IGraphicalRepresentation.
 */
class EmberEntity: public Eris::ViewEntity, public IVisualizable
{
public:

	/**
	 * @brief The different positioning modes the entity can be in.
	 * The positioning mode determines how the entity can be adjusted in the world.
	 */
	enum class PositioningMode
	{
		/**
		 * @brief The default mode, where the entity is affected by physics.
		 */
		FREE,

		/**
		 * @brief Floating on top of an entity. Usually a mass of water.
		 */
		FLOATING,

		/**
		 * @brief Planted to the ground
		 */
		PLANTED,

		/**
		 * @brief Fixed in the world. This means that the position of the entity won't be adjusted (i.e. not affected by gravity etc.).
		 */
		FIXED,

		/**
		 * @brief Submerged
		 */
		SUBMERGED,

		/**
		 * @brief Projectile
		 */
		PROJECTILE
	};

	/**
	 * @brief The different composition modes available.
	 * Some entities can be "composition" entities. That means that they and their children make up one single conceptual entity.
	 * An example might be a table, where each table leg is a separate entity. Each leg is then part of the "table composition".
	 */
	enum CompositionMode
	{
		CM_DISABLED, //!< CM_DISABLED No composition is used. This should be the default.
		CM_COMPOSITION, //!< CM_COMPOSITION The entity is an composite. That means that when interacting with a child entity the user should also be given the option to interact with the parent composite entity.
		CM_COMPOSITION_EXCLUSIVE //!< CM_COMPOSITION_EXCLUSIVE The entity is an exclusive composite. That means that when interacting with a child entity the user should only be presented with the parent composite entity (and should not be able to interact with the child entity).
	};

	/**
	 * @brief Ctor.
	 * This isn't normally called by something else than the EmberEntityFactory, but we'll keep it public for testing sake.
	 * @param id The unique id of the entity.
	 * @param ty The type info which specified the type for this entity.
	 * @param vw The world view to which this entity belongs.
	 */
	EmberEntity(std::string id, Eris::TypeInfo* ty, Eris::View& vw);

	/**
	 * @brief Dtor.
	 */
	~EmberEntity() override;

	/**
	 * @brief Adjust the height of the entity so that it "snaps" to the ground or is otherwise adjusted, depending on the current movement mode.
	 * This is most often done by making a call to the containing node's adjustPositionForContainedNode method.
	 * @see adjustPositionForContainedNode()
	 */
	void adjustPosition();

	/**
	 * @brief Returns true if the entity has a list of suggested responses to something this entity has said in the world.
	 * @see getSuggestedResponses()
	 * @return True if there's a collection of suggested responses to something that this entity has just said.
	 */
	bool hasSuggestedResponses() const;

	/**
	 * @brief Returns a list of all suggested responses when conversing with this entity.
	 * When talking to entities in the world (for example NPCs) the can sometimes be a specified list of suggested responses.
	 * @return A vector of the suggested responses.
	 */
	const std::vector<std::string>& getSuggestedResponses() const;

	/**
	 * @brief Gets the location (i.e. parent entity) as cast to an EmberEntity.
	 * This behaves just like getLocation, but will always return an instance of EmberEntity. We can do this since we're guaranteed to only ever deal with instances of EmberEntity in the world, thanks to the EmberEntityFactory class.
	 * @see Eris::Entity::getLocation
	 * @see OgreView::EmberEntityFactory
	 * @return The location of this entity, i.e. the parent entity. If there's no parent entity available, null will be returned (i.e. if the entity is in limbo, or if it's the topmost entity).
	 */
	EmberEntity* getEmberLocation() const;

	/**
	 * @brief Gets the child entity with the specified index.
	 * @note This works just like Eris::Entity::getContained, only it returns EmberEntity instances directly (thus removing the need for casting).
	 * @param index The index of the child entity.
	 * @return A pointer to a contained entity.
	 */
	EmberEntity* getEmberContained(size_t index) const;

	/**
	 * @brief Returns true if init(...) has been called and the entity been set up.
	 * This can be useful when handling the entity, and not knowing whether it has been properly initialized yet. After the entity has been created, there's a small delay until init() is called. During this time the entity is in an incomplete state, and code that accesses it during that time might instead want to wait until it has been properly initialized.
	 * @see init()
	 * @return True if init() already has been called, else false.
	 */
	bool isInitialized() const;

	/**
	 * @brief The movement mode the entity is in, like walking, running, swimming etc.
	 * @return The current movement mode of the entity.
	 */
	PositioningMode getPositioningMode() const;


	/**
	 * @brief Dumps all of this entity's attributes to the supplied outstream.
	 * By default the entity will be exported using XML Atlas format.
	 * @param outstream The stream to write to.
	 * @param logOutstream The stream to which log messages will be written.
	 */
	void dumpAttributes(std::iostream& outstream, std::ostream& logOutstream) const;

	/**
	 * General method for turning on and off debug visualizations. Subclasses might support more types of visualizations than the ones defined here.
	 * @param visualization The type of visualization. Currently supports "OgreBBox".
	 * @param visualize Whether to visualize or not.
	 */
	void setVisualize(const std::string& visualization, bool visualize) override;

	/**
	 * @brief Gets whether a certain visualization is turned on or off.
	 * @param visualization The type of visualization. Currently supports "OgreBBox".
	 * @return true if visualization is turned on, else false
	 */
	bool getVisualize(const std::string& visualization) const override;

	/**
	 * @brief Gets the graphical representation of this entity.
	 * Not all entities will have graphical representations, so you must always make sure to check for null on the result.
	 * Most graphical representation classes will also contain methods for safely casting into the concrete subclass.
	 * @returns The graphical representation, or null if there are none.
	 */
	IGraphicalRepresentation* getGraphicalRepresentation() const;

	/**
	 * @brief Emitted when the positioning mode has changed.
	 * The entity is placed in the world differently depending on the "positioning mode". The default mode is to be gravity affected, i.e. fall to the ground.
	 * In a simplified simulation this will in most cases mean that the entity is snapped to the terrain. However, there are other options, such as "fixed" or "floating".
	 * The parameter sent is the new positioning mode.
	 * This event will be emitted before the actual mode is changed, so you can call getPositioningMode() to get the current positioning mode, before the new one is in effect.
	 */
	sigc::signal<void, PositioningMode> EventPositioningModeChanged;

	/**
	 * @brief Sets the attachment to use.
	 * @param attachment The new attachment to use. Ownership will be transferred to this instance.
	 */
	void setAttachment(std::unique_ptr<IEntityAttachment> attachment);

	/**
	 * @brief Gets the attachment used for this entity.
	 * @return The attachment used by this entity, or null if there is no attachment.
	 */
	IEntityAttachment* getAttachment() const;

	/**
	 * @brief Sets a control delegate for the attachment.
	 * The attachment instance, used to bind the graphical representation to the parent entity, normally directly accesses the position and orientation data from the entity.
	 * However, in some cases you would want to provide the position and orientation data differently, for example when you want to alter the position of the entity on the client, independent of the server.
	 * By using an instance of IEntityControlDelegate and setting it on the entity through this method you can achieve this.
	 * @param delegate The delegate through which all queries about the position and orientation of the entity will go.
	 */
	void setAttachmentControlDelegate(IEntityControlDelegate* delegate);

	/**
	 * @brief Gets the control delegate for the attachment.
	 * @return A pointer to the currently used attachment controller delegate, or null if none is used.
	 */
	IEntityControlDelegate* getAttachmentControlDelegate() const;

	/**
	 * @brief Gets the entity attached to the named point.
	 * @param attachment A named point.
	 * @return A child entity, or null if none found.
	 */
	EmberEntity* getAttachedEntity(const std::string& attachment);

	/**
	 * @brief Accepts a visitor.
	 * This is used to allow for instances of IEntityVisitor to traverse the entity hierarchy.
	 * @param visitor The visitor.
	 */
	void accept(IEntityVisitor& visitor);

	/**
	 * @brief Accepts a visitor function.
	 * This is used to allow for easy traversal of the entity hierarchy.
	 * @param visitor The visitor function. If the function returns true child entities will be traversed too.
	 */
	void accept(std::function<bool(const EmberEntity&)>& visitor) const;

	/**
	 * @brief Accepts a visitor function.
	 * This is used to allow for easy traversal of the entity hierarchy.
	 * @param visitor The visitor function. If the function returns true child entities will be traversed too.
	 */
	void accept(std::function<bool(EmberEntity&)>& visitor);

	/**
	 * @brief Gets the height at the local position.
	 * @param localPosition A position local to the entity.
	 * @return The height at the location.
	 */
	virtual float getHeight(const WFMath::Point<2>& localPosition) const;

	std::string getNameOrType() const;

	void setHeightProvider(IHeightProvider* heightProvider);

	const std::map<std::string, Eris::Usage>& getUsages() const;
	const std::map<std::string, Eris::Usage>& getUsagesProtected() const;

	CompositionMode getCompositionMode() const;
	void setCompositionMode(CompositionMode mode);

	sigc::signal<void, const EntityTalk&> EventTalk;

	/**
	 * @brief Emitted when the graphical representation changes.
	 */
	sigc::signal<void> EventChangedGraphicalRepresentation;

	sigc::signal<void> EventAttachmentChanged;

	/**
	 * Registers a global attribute change listener.
	 *
	 * This is an attribute listener which will be called for _all_ entities.
	 * @param attributeName The attribute to listen for changes to.
	 * @param listener
	 */
	static void registerGlobalAttributeListener(const std::string& attributeName, std::function<void(EmberEntity&, const Atlas::Message::Element&)>& listener);

	/**
	 * De-registers a global attribute change listener.
	 *
	 * @param attributeName The attribute to listen for changes to.
	 * @param listener
	 */
	static void deregisterGlobalAttributeListener(const std::string& attributeName, std::function<void(EmberEntity&, const Atlas::Message::Element&)>& listener);

protected:

	/**
	 * @brief If this is true, init(...) has been called and the entity been set up.
	 * @see isInitialized().
	 */
	bool mIsInitialized;

	/**
	 @brief Sometimes when talking to an entity, the server will provide suggested responses. These are stored here.
	 */
	std::vector<std::string> mSuggestedResponses;

	std::map<std::string, Eris::Usage> mUsages;
	std::map<std::string, Eris::Usage> mUsagesProtected;

	/**
	 * @brief The positioning mode the entity is in, like gravity affected, fixed or floating.
	 */
	PositioningMode mPositioningMode;

	/**
	 * @brief The composition mode for the entity.
	 *
	 * Is "none" (CM_DISABLED) by default.
	 */
	CompositionMode mCompositionMode;

	/**
	 * @brief The attachment for this entity.
	 *
	 * Since the graphical representation of an entity can be expressed in many different way, that is handled by an instance of IEntityAttachment and not by the entity itself.
	 */
	std::unique_ptr<IEntityAttachment> mAttachment;

	/**
	 * @brief An attachment control delegate used by this entity.
	 * Normally the entity attachment will use the position and orientation of the entity when determining where to position the graphical representation.
	 * However, sometimes you want to override that with other data. This instance can be used by the attachment for this purpose.
	 */
	IEntityControlDelegate* mAttachmentControlDelegate;

	/**
	 * @brief An optional height provider attached to this entity.
	 */
	IHeightProvider* mHeightProvider;

	/**
	 *    @copydoc Eris::Entity::onTalk()
	 */
	void onTalk(const Atlas::Objects::Operation::RootOperation& talk) override;
	/**
	 *    @copydoc Eris::Entity::onLocationChanged()
	 */
	void onLocationChanged(Eris::Entity *oldLocation) override;
	/**
	 *    @copydoc Eris::Entity::onAction()
	 */
	void onAction(const Atlas::Objects::Operation::RootOperation& act) override;
	/**
	 *    @copydoc Eris::Entity::onHit()
	 */
	void onHit(const Atlas::Objects::Operation::Hit& hit) override;
	/**
	 *    @copydoc Eris::Entity::onSoundAction()
	 */
	void onSoundAction(const Atlas::Objects::Operation::RootOperation& op) override;

	/**
	 *    @copydoc Eris::Entity::onPropertyChanged()
	 */
	void onPropertyChanged(const std::string& str, const Atlas::Message::Element& v) override;

	/**
	 * @brief Called when the positioning mode of the entity changes.
	 * Depending on the kind of positioning mode the entity needs to be adjusted in the world accordinly. For example, when the entity is gravity affected (the default) it should snap to the terrain, in contrast to when the mode is "fixed" where it wouldn't snap to the terrain.
	 * @param newMode The new movement mode.
	 */
	virtual void onPositioningModeChanged(PositioningMode newMode);

	/**
	 * @brief Called when the bounding box has changed.
	 */
	virtual void onBboxChanged();

	/**
	 * @brief Called by eris just after the entity has been put into the world.
	 * @param ge The root entity which contains all atlas data that define this entity.
	 * @param fromCreateOp
	 */
	void init(const Atlas::Objects::Entity::RootEntity &ge, bool fromCreateOp) override;

	/**
	 * @brief Parses the current positioning mode from the submitted element, which should be taken from the "mode" attribute.
	 * This method will in turn call onPositioningModeChanged if the mode is changed
	 */
	void parsePositioningModeChange(const std::string& mode);

	/**
	 * @brief Re-attaches all child entities.
	 * This will iterate through all child entities recursively, and ask them to reattach themselves (i.e. create a new attachment).
	 * You usually call this when the attachment has changed, and the child attachments needs to be updated.
	 */
	void reattachChildren();

	/**
	 * @brief Updates the attachment.
	 * Call this when the location changes or if something else causes the graphical representation to change.
	 */
	void updateAttachment();

	void parseUsages(std::map<std::string, Eris::Usage>& map, const Atlas::Message::Element& element);
};

inline bool EmberEntity::isInitialized() const
{
	return mIsInitialized;
}

inline EmberEntity::PositioningMode EmberEntity::getPositioningMode() const
{
	return mPositioningMode;
}


}

#endif // EMBEROGRE_EMBERENTITY_H
