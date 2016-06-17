/*
 * Ork: a small object-oriented OpenGL Rendering Kernel.
 * Copyright (c) 2008-2010 INRIA
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.
 */

/*
 * Authors: Eric Bruneton, Antoine Begault, Guillaume Piolat.
 */

#include "ork/ui/EventHandler.h"

namespace ork
{

EventHandler::EventHandler(const char *type) : Object(type)
{
}

EventHandler::~EventHandler()
{
}

void EventHandler::redisplay(double t, double dt)
{
}

void EventHandler::reshape(int x, int y)
{
}

void EventHandler::idle(bool damaged)
{
}

bool EventHandler::mouseClick(button b, state s, modifier m, int x, int y)
{
    return false;
}

bool EventHandler::mouseWheel(wheel b, modifier m, int x, int y)
{
    return false;
}

bool EventHandler::mouseMotion(int x, int y)
{
    return false;
}

bool EventHandler::mousePassiveMotion(int x, int y)
{
    return false;
}

bool EventHandler::keyTyped(unsigned char c, modifier m, int x, int y)
{
    return false;
}

bool EventHandler::keyReleased(unsigned char c, modifier m, int x, int y)
{
    return false;
}

bool EventHandler::specialKey(key  k, modifier m, int x, int y)
{
    return false;
}

bool EventHandler::specialKeyReleased(key  k, modifier m, int x, int y)
{
    return false;
}

}
