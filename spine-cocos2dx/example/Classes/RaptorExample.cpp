/******************************************************************************
 * Spine Runtimes License Agreement
 * Last updated September 24, 2021. Replaces all prior versions.
 *
 * Copyright (c) 2013-2021, Esoteric Software LLC
 *
 * Integration of the Spine Runtimes into software or otherwise creating
 * derivative works of the Spine Runtimes is permitted under the terms and
 * conditions of Section 2 of the Spine Editor License Agreement:
 * http://esotericsoftware.com/spine-editor-license
 *
 * Otherwise, it is permitted to integrate the Spine Runtimes into software
 * or otherwise create derivative works of the Spine Runtimes (collectively,
 * "Products"), provided that each user of the Products must obtain their own
 * Spine Editor license and redistribution of the Products in any form must
 * include this license and copyright notice.
 *
 * THE SPINE RUNTIMES ARE PROVIDED BY ESOTERIC SOFTWARE LLC "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ESOTERIC SOFTWARE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES,
 * BUSINESS INTERRUPTION, OR LOSS OF USE, DATA, OR PROFITS) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THE SPINE RUNTIMES, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include "RaptorExample.h"
#include "TankExample.h"
#include <spine/Extension.h>

USING_NS_CC;
using namespace spine;

PowInterpolation pow2(2);
PowOutInterpolation powOut2(2);
SwirlVertexEffect effect(400, powOut2);

Scene *RaptorExample::scene() {
	Scene *scene = Scene::create();
	scene->addChild(RaptorExample::create());
	return scene;
}

bool RaptorExample::init() {
	if (!LayerColor::initWithColor(Color4B(128, 128, 128, 255))) return false;

	skeletonNode = SkeletonAnimation::createWithJsonFile("raptor-pro.json", "raptor.atlas", 0.5f);
	skeletonNode->setAnimation(0, "walk", true);
	skeletonNode->addAnimation(1, "gun-grab", false, 2);
	skeletonNode->setTwoColorTint(true);

	effect.setCenterY(200);
	swirlTime = 0;

	skeletonNode->setVertexEffect(&effect);

	skeletonNode->setPosition(Vec2(_contentSize.width / 2, 20));
	addChild(skeletonNode);

	scheduleUpdate();

	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch *touch, cocos2d::Event *event) -> bool {
		if (!skeletonNode->getDebugBonesEnabled()) {
			skeletonNode->setDebugBonesEnabled(true);
			skeletonNode->setDebugMeshesEnabled(true);
		} else if (skeletonNode->getTimeScale() == 1)
			skeletonNode->setTimeScale(0.3f);
		else
			Director::getInstance()->replaceScene(TankExample::scene());
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void RaptorExample::update(float fDelta) {
	swirlTime += fDelta;
	float percent = spine::MathUtil::fmod(swirlTime, 2);
	if (percent > 1) percent = 1 - (percent - 1);
	effect.setAngle(pow2.interpolate(-60.0f, 60.0f, percent));
}
