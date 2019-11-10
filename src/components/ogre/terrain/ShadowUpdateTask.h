/*
 Copyright (C) 2010 Erik Ogenvik <erik@ogenvik.org>

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

#ifndef SHADOWUPDATETASK_H_
#define SHADOWUPDATETASK_H_

#include "framework/tasks/TemplateNamedTask.h"
#include "Types.h"
#include <wfmath/vector.h>

namespace Ember {
namespace OgreView {

namespace Terrain {

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Async task for updating precomputed shadows for pages.
 * This is only of use when using the fixed function pipeline.
 */
class ShadowUpdateTask : public Tasks::TemplateNamedTask<ShadowUpdateTask> {
public:
	ShadowUpdateTask(GeometryPtrVector pageGeometries, const WFMath::Vector<3>& lightDirection);

	~ShadowUpdateTask() override;

	void executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context) override;

	bool executeTaskInMainThread() override;

private:

	GeometryPtrVector mPageGeometries;

	const WFMath::Vector<3> mLightDirection;
};

}

}

}

#endif /* SHADOWUPDATETASK_H_ */
