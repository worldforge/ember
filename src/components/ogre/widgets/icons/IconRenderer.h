//
// C++ Interface: IconRenderer
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBEROGRE_GUI_ICONSICONRENDERER_H
#define EMBEROGRE_GUI_ICONSICONRENDERER_H

#include <string>
#include <memory>
#include "components/ogre/EmberOgrePrerequisites.h"
#include <queue>
#include <OgreFrameListener.h>
#include <components/ogre/SceneNodeProvider.h>

namespace Ember {
namespace OgreView {

class SimpleRenderContext;

namespace Model
{
	class Model;
}

namespace Gui {

namespace Icons {

class Icon;
class IconRenderer;
class IconImageStoreEntry;
class DelayedIconRendererWorker;

/**
An entry belonging to the DelayedIconRendererWorker.
*/
class DelayedIconRendererEntry
{
public:
	DelayedIconRendererEntry(DelayedIconRendererWorker& renderer, const std::shared_ptr<Model::Model>& model, Icon* icon);
	virtual ~DelayedIconRendererEntry();
	/**
	 *    Accessor for the model which will be rendered.
	 * @return
	 */
	Model::Model* getModel();
	/**
	 *    Accessor for the icon.
	 * @return
	 */
	Icon* getIcon();

	/**
	 *    Called by the DelayedIconRendererWorker at the start of each frame.
	 */
	void frameStarted();

protected:
	/**
	A reference to the owner instance.
	*/
	DelayedIconRendererWorker& mRenderer;
	std::shared_ptr<Model::Model> mModel;
	Icon* mIcon;
	/**
	We'll keep track of the number of frames for this instance. On the first frame we'll perform the rendering, and on later frames we'll blit the texture.
	*/
	unsigned int mFrames;

};


/**
The abstract class which performs the actual rendering.
Note that it's the responsibility of this class to make sure that the Model supplied in the render method is properly destroyed.
*/
class IconRenderWorker
{
public:
	IconRenderWorker(IconRenderer& renderer);
	virtual ~IconRenderWorker();
    /**
     * Starts the process of rendering the model onto the icon. Depending on the implementation the actual blitting and rendering might be delayed some frames.
     * @param model The model to render. Note that it's the responsibility of this class to make sure that's it's properly destroyed after use.
     * @param icon
     */
    virtual void render(const std::shared_ptr<Model::Model>& model, Icon* icon, IconImageStoreEntry* imageStoreEntry) = 0;
protected:
	IconRenderer& mRenderer;
	IconImageStoreEntry* mImageStoreEntry;
};

/**
Renders with a delay between the rendering and the blitting, thus allowing the GPU to perform the rendering.
*/
class DelayedIconRendererWorker : public Ogre::FrameListener, public IconRenderWorker
{
friend class DelayedIconRendererEntry;
public:
	DelayedIconRendererWorker(IconRenderer& renderer);

	virtual ~DelayedIconRendererWorker();

	/**
	* Calles by Ogre. At each frame we'll see if there's any entries in our queue. If so, we'll call frameStarted on the first entry in the list.
	*/
	bool frameStarted(const Ogre::FrameEvent& event);

    /**
     * Starts the process of rendering a model onto an icon. The blitting will be delayed a couple of frames though.
     * @param model
     * @param icon
     */
    void render(const std::shared_ptr<Model::Model>& model, Icon* icon, IconImageStoreEntry* imageStoreEntry) override;


protected:

	IconRenderer& getRenderer();

	/**
	 *    Method to be called by the contained entries when they want to perform the actual rendering operation.
	 * @param entry
	 */
	void performRendering(DelayedIconRendererEntry& entry);

	/**
	 *    Method to be called by the contained entries when they want the blitting to occur.
	 *    Calling this method will also pop an entry from the entries queue. (So if you call it from within a DelayedIconRendererEntry entry, make sure to not touch any member variables afterwards, since the instance will have been destroyed.)
	 * @param entry
	 */
	void finalizeRendering(DelayedIconRendererEntry& entry);

	typedef std::queue<DelayedIconRendererEntry> EntryStore;

	/**
	The queue of entries which will be rendered in order.
	*/
	EntryStore entries;

};

/**
An instance of this class will render the icon and blit it to texture all in the same frame.
*/
class DirectRendererWorker : public IconRenderWorker
{
public:
	DirectRendererWorker(IconRenderer& renderer);
	virtual ~DirectRendererWorker();

    /**
     * Starts the process of rendering a model onto an icon. The blitting will occur in the same frame as the rendering.
     * @param model
     * @param icon
     */
     void render(const std::shared_ptr<Model::Model>& model, Icon* icon, IconImageStoreEntry* imageStoreEntry) override;
};

/**
	@author Erik Ogenvik <erik@ogenvik.org>
	Responsible for rendering the model to the icon texture.
	The actual rendering will be handled by an instance of IconRenderWorker.
	Note that it's not guaranteed that the rendering and blitting will occur on the same frame.
*/
class IconRenderer
{
friend class IconRenderWorker;
public:

    /**
     * Ctor. Be sure to call setWorker before you start any rendering.
     * @param prefix
     * @param pixelWidth
     */
    IconRenderer(const std::string& prefix, int pixelWidth);

    ~IconRenderer();

    /**
     * Renders a model by the specified name to the icon.
     * @param modelName The name of the model to render.
     * @param icon The icon it should be rendered to.
     */
    void render(const std::string& modelName, Icon* icon);

	/**
	* Renders the Model onto the Icon. Note that depending on the IconRenderWorker used this might not occur on the same frame.
	* Make sure to call setWorker before calling this method.
	* @param model The Model to render. Note that after calling this ownership of the Model is transferred to this instance, which takes care of the proper destruction of it.
	* @param icon The icon to render to.
	*/
	void render(const std::shared_ptr<Model::Model>& model, Icon* icon);

    /**
     * Gets the SimpleRenderContext used for the rendering.
     * @return
     */
    SimpleRenderContext* getRenderContext();

    /**
     * Sets the worker instance. Be sure to call this before doing any rendering.
     * @param worker
     */
    void setWorker(IconRenderWorker* worker);


    /**
     * Performs the actual rendering op.
     * @param model
     * @param icon
     */
    void performRendering(Model::Model* model, Icon* icon);


    /**
     * Blits the rendered texture onto the icon texture.
     * @param icon
     */
    void blitRenderToIcon(Icon* icon);

protected:


	int mPixelWidth;
	std::unique_ptr<SimpleRenderContext> mRenderContext;
	IconRenderWorker* mWorker;
	SceneNodeProvider mSceneNodeProvider;

	/**
	 * @brief Call this when the model is being rendered in a backround thread, and we want to render it to the icon once it's done.
	 * @param model The model.
	 * @param icon The icon.
	 */
	void renderDelayed(const std::shared_ptr<Model::Model>& model, Icon* icon);
};

}

}

}

}

#endif
