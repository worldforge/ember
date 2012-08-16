#include "sigc++/connection.h"

#include "string"

namespace Ember
{
namespace OgreView
{
class IGraphicalChangeAdapter;
class ShaderManager;

/**
 * @brief Handles changes in shader level.
 * Acts as a subcomponent of the automatic handling of graphics system.
 */
class ShaderLevelManager
{
public:
	/**
	 * @brief Constructor.
	 */
	ShaderLevelManager(IGraphicalChangeAdapter& iGraphicalChangeAdapter, ShaderManager& shaderManager);

	/**
	 * @brief Destructor.
	 */
	~ShaderLevelManager();

	/**
	 * @brief Initializes the material lod level manager.
	 * The manager starts listening for graphics detail change requests.
	 */
	void initialize();

	/**
	 * @brief Sets the shader scheme to the required level.
	 * @param level The needed shader level (high, medium, low).
	 * If the level doesn't exist, it will switch to default.
	 */
	void changeShaderLevel(const std::string& level);

	/**
	 * @brief This can be used to stop this component responding to change requests.
	 */
	void pause();

	/**
	 * @brief This can be used to restart this component responding to change requests.
	 */
	void unpause();

protected:
	/**
	 * @brief Checks level against this component's threshold value to determine change in material lod.
	 * This function is used to listen to the changes required by the automatic graphics detail system.
	 */
	bool changeLevel(float level);

	/**
	 * @brief Steps up the shader scheme level.
	 * @returns Whether shader scheme could be stepped up.
	 */
	bool stepUpShaderLevel();

	/**
	 * @brief Steps down the shader scheme level.
	 * @returns Whether shader scheme could be stepped down.
	 */
	bool stepDownShaderLevel();

	/**
	 * The maximum shader level this component can switch to.
	 */
	std::string mMaxShaderLevel;

	/**
	 * The minimum shader level this component can switch to.
	 */
	std::string mMinShaderLevel;

	/**
	 * The current shader level.
	 */
	std::string mShaderLevel;

	/**
	 * The threshold fps change after which the shader detail is changed to respond to performance change requested.
	 */
	float mShaderThresholdLevel;

	/**
	 * Holds the reference to the connection to the changeRequired signal. Used to disconnect the signal on destruction of this class or to pause the functioning of this component.
	 */
	sigc::connection mChangeRequiredConnection;

	/**
	 * Reference to the graphical change adapter through which graphics detail changes are requested.
	 */
	IGraphicalChangeAdapter& mIGraphicalChangeAdapter;

	/**
	 * Reference to the Shader Manager used to make scheme changes.
	 */
	ShaderManager& mShaderManager;

};

}
}
