#ifndef HAPTICSPHERE_H
#define HAPTICSPHERE_H

#include "Assignment.h"

#include "chai3d.h"

class HapticSphere : public Assignment
{
private:
    // A 3D cursor for the haptic device
    cShapeSphere* m_cursor;

    // Material properties used to render the color of the cursors
    cMaterialPtr m_matCursorButtonON;
    cMaterialPtr m_matCursorButtonOFF;

public:
    virtual std::string getName() const { return "6: Haptic Sphere"; }

	virtual void initialize(cWorld* world, cCamera* camera);
	virtual void updateGraphics();
	virtual void updateHaptics(cGenericHapticDevice* hapticDevice, double timeStep, double totalTime);
};

void HapticSphere::initialize(cWorld* world, cCamera* camera)
{
	//Change the background
	world->setBackgroundColor(0.2f, 0, 0.31f);

	// Create a cursor with its radius set
    m_cursor = new cShapeSphere(0.003);
	// Add cursor to the world
	world->addChild(m_cursor);

	// Here we define the material properties of the cursor when the
	// user button of the device end-effector is engaged (ON) or released (OFF)

    // A light orange material color
    m_matCursorButtonOFF = cMaterialPtr(new cMaterial());
    m_matCursorButtonOFF->m_ambient.set(0.5, 0.2, 0.0);
    m_matCursorButtonOFF->m_diffuse.set(1.0, 0.5, 0.0);
    m_matCursorButtonOFF->m_specular.set(1.0, 1.0, 1.0);

    // A blue material color
    // A light orange material color
    m_matCursorButtonON = cMaterialPtr(new cMaterial());
    m_matCursorButtonON->m_ambient.set(0.1, 0.1, 0.4);
    m_matCursorButtonON->m_diffuse.set(0.3, 0.3, 0.8);
    m_matCursorButtonON->m_specular.set(1.0, 1.0, 1.0);

    // Apply the 'off' material to the cursor
    m_cursor->m_material = m_matCursorButtonOFF;

    // draw a sphere
    cShapeSphere *mySphere = new cShapeSphere(0.015);
    world->addChild(mySphere);
}

void HapticSphere::updateGraphics()
{

}

void HapticSphere::updateHaptics(cGenericHapticDevice* hapticDevice, double timeStep, double totalTime)
{
	//Read the current position of the haptic device
	cVector3d newPosition;
	hapticDevice->getPosition(newPosition);

    // update global variable for graphic display update
    hapticDevicePosition = newPosition;

	// Update position and orientation of cursor
    m_cursor->setLocalPos(newPosition);

	cVector3d force(0, 0, 0);
	//Set a force to the haptic device
    cVector3d forceDir = newPosition;
    forceDir.normalize();
    double forceMag;
    double distance = newPosition.length();
    double rSphere = 0.015;
    double rCursor = 0.003;

    bool forceStatus;
    bool proxyStatus;
    if(distance >= rSphere + rCursor)
    {
        forceStatus = false;
    }
    else
    {
        forceStatus = true;
    }
    proxyStatus = forceStatus;
    m_cursor->m_material = forceStatus ? m_matCursorButtonON : m_matCursorButtonOFF;

    //TODO: Add forces which simulates a wall.
    if(forceStatus)
    {
        forceMag = 3000 * (rSphere + rCursor - distance);
        force = (forceMag * forceDir);

        // proxy idea
        cVector3d proxyPos;
        proxyPos = (rSphere + rCursor) * forceDir;
        m_cursor->setLocalPos(proxyPos);
    }

	hapticDevice->setForce(force);
}
#endif

