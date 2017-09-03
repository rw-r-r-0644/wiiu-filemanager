/****************************************************************************
 * Copyright (C) 2015 Dimok
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include "GuiButton.h"
#include "GuiTrigger.h"
#include "GuiController.h"
#include "dynamic_libs/vpad_functions.h"

/**
 * Constructor for the GuiButton class.
 */

GuiButton::GuiButton(f32 w, f32 h)
{
    width = w;
    height = h;
	image = NULL;
	imageOver = NULL;
	imageHold = NULL;
	imageClick = NULL;
	icon = NULL;
	iconOver = NULL;

	for(int i = 0; i < 4; i++)
	{
		label[i] = NULL;
		labelOver[i] = NULL;
		labelHold[i] = NULL;
		labelClick[i] = NULL;
	}
	for(int i = 0; i < iMaxGuiTriggers; i++)
	{
		trigger[i] = NULL;
	}

	soundOver = NULL;
	soundHold = NULL;
	soundClick = NULL;
	clickedTrigger = NULL;
	heldTrigger = NULL;
	selectable = true;
	holdable = true;
	clickable = true;
}

/**
 * Destructor for the GuiButton class.
 */
GuiButton::~GuiButton()
{
	if(rumblePattern) free(rumblePattern);
}

void GuiButton::setImage(GuiImage* img)
{
	image = img;
	if(img) img->setParent(this);
}
void GuiButton::setImageOver(GuiImage* img)
{
	imageOver = img;
	if(img) img->setParent(this);
}
void GuiButton::setImageHold(GuiImage* img)
{
	imageHold = img;
	if(img) img->setParent(this);
}
void GuiButton::setImageClick(GuiImage* img)
{
	imageClick = img;
	if(img) img->setParent(this);
}
void GuiButton::setIcon(GuiImage* img)
{
	icon = img;
	if(img) img->setParent(this);
}
void GuiButton::setIconOver(GuiImage* img)
{
	iconOver = img;
	if(img) img->setParent(this);
}

void GuiButton::setLabel(GuiText* txt, int n)
{
	label[n] = txt;
	if(txt) txt->setParent(this);
}
void GuiButton::setLabelOver(GuiText* txt, int n)
{
	labelOver[n] = txt;
	if(txt) txt->setParent(this);
}
void GuiButton::setLabelHold(GuiText* txt, int n)
{
	labelHold[n] = txt;
	if(txt) txt->setParent(this);
}
void GuiButton::setLabelClick(GuiText* txt, int n)
{
	labelClick[n] = txt;
	if(txt) txt->setParent(this);
}
void GuiButton::setSoundOver(GuiSound * snd)
{
	soundOver = snd;
}
void GuiButton::setSoundHold(GuiSound * snd)
{
	soundHold = snd;
}

void GuiButton::setSoundClick(GuiSound * snd)
{
	soundClick = snd;
}

void GuiButton::setHoldLimit(u64 ms)
{
	heldLimit = ms;
}

void GuiButton::setRumble(f32 len)
{
	rumbleLenght = u8(len * 120 / 8);
	rumblePattern = (u8*)memset(malloc(rumbleLenght), 0xFF, rumbleLenght); // not checking for failed malloc
}

void GuiButton::setTrigger(GuiTrigger * t, int idx)
{
    if(idx >= 0 && idx < iMaxGuiTriggers)
    {
        trigger[idx] = t;
    }
    else
    {
        for(int i = 0; i < iMaxGuiTriggers; i++)
        {
            if(!trigger[i])
            {
                trigger[i] = t;
                break;
            }
        }
    }
}

void GuiButton::resetState(void)
{
	clickedTrigger = NULL;
    heldTrigger = NULL;
    GuiElement::resetState();
}

/**
 * Draw the button on screen
 */
void GuiButton::draw(CVideo *v)
{
	if(!this->isVisible())
		return;

	// draw image
	if((isDrawOverOnlyWhenSelected() && (isStateSet(STATE_SELECTED) && imageOver)) ||
      (!isDrawOverOnlyWhenSelected() && (isStateSet(STATE_OVER | STATE_SELECTED | STATE_CLICKED | STATE_HELD) && imageOver)))
		imageOver->draw(v);
	else if(image)
		image->draw(v);

	if((isDrawOverOnlyWhenSelected() && (isStateSet(STATE_SELECTED) && iconOver)) ||
      (!isDrawOverOnlyWhenSelected() && (isStateSet(STATE_OVER | STATE_SELECTED | STATE_CLICKED | STATE_HELD) && iconOver)))
		iconOver->draw(v);
	else if(icon)
		icon->draw(v);

	// draw text
	for(int i = 0; i < 4; i++)
	{
		if(isStateSet(STATE_OVER | STATE_SELECTED | STATE_CLICKED | STATE_HELD) && labelOver[i])
			labelOver[i]->draw(v);
		else if(label[i])
			label[i]->draw(v);
	}
}

void GuiButton::update(GuiController * c)
{
	if(!c || isStateSet(STATE_DISABLED|STATE_HIDDEN|STATE_DISABLE_INPUT, c->chan))
		return;
	else if(parentElement && (parentElement->isStateSet(STATE_DISABLED|STATE_HIDDEN|STATE_DISABLE_INPUT, c->chan)))
		return;

    if(selectable)
    {
		if(c->data.validPointer && this->isInside(c->data.x, c->data.y))
		{
			if(!isStateSet(STATE_OVER, c->chan))
			{
			    setState(STATE_OVER, c->chan);

				if(effectsOver && !effects)
				{
					// initiate effects
					effects = effectsOver;
					effectAmount = effectAmountOver;
					effectTarget = effectTargetOver;
				}

                pointedOn(this, c);
			}
		}
		else if(isStateSet(STATE_OVER, c->chan))
        {
            this->clearState(STATE_OVER, c->chan);
            pointedOff(this, c);

			if(effectTarget == effectTargetOver && effectAmount == effectAmountOver)
			{
				// initiate effects (in reverse)
				effects = effectsOver;
				effectAmount = -effectAmountOver;
				effectTarget = 100;
			}
        }
    }

    for(int i = 0; i < iMaxGuiTriggers; i++)
    {
        if(!trigger[i])
            continue;

        // button triggers
        if(clickable)
        {

            int isClicked = trigger[i]->clicked(c);

            if(   !clickedTrigger && (isClicked != GuiTrigger::CLICKED_NONE)
               && (trigger[i]->isClickEverywhere() || (isStateSet(STATE_SELECTED | STATE_OVER, c->chan) && trigger[i]->isSelectionClickEverywhere()) || this->isInside(c->data.x, c->data.y)))
            {


                clickedTrigger = trigger[i];

                if(!isStateSet(STATE_CLICKED, c->chan)){
                    if(isClicked == GuiTrigger::CLICKED_TOUCH)
                        setState(STATE_CLICKED_TOUCH, c->chan);
                    else
                        setState(STATE_CLICKED, c->chan);
                }

				//! the button was just pressed, start counting how many ms it's keept pressed
				heldStart = OSGetTime() / MILLISECS_TO_TICKS(1);
				pressedLock = 0;

				//! kinda hacky to check for scrolling here but it works...
				heldStartXPos = getOffsetX();
				heldStartYPos = getOffsetY();
            }
            else if((isStateSet(STATE_CLICKED, c->chan) || isStateSet(STATE_CLICKED_TOUCH, c->chan)) && (clickedTrigger == trigger[i]) && !isStateSet(STATE_HELD, c->chan) && !trigger[i]->held(c) && ((isClicked == GuiTrigger::CLICKED_NONE) || trigger[i]->released(c)))
            {
                if((isStateSet(STATE_CLICKED_TOUCH, c->chan) && this->isInside(c->data.x, c->data.y)) || (isStateSet(STATE_CLICKED, c->chan))){
                    clickedTrigger = NULL;
                    clearState(STATE_CLICKED, c->chan);
					
                    //! if the button was held more than the limit it already sent a pressedLong event
					if (((OSGetTime() / MILLISECS_TO_TICKS(1)) - heldStart) <= heldLimit && abs(heldStartXPos - getOffsetX()) < 20 && abs(heldStartYPos - getOffsetY()) < 20)
					{
						//! play the sound after the button is released
						if(soundClick)
							soundClick->Play();
						
						//! send the pressed event
						pressed(this, c, trigger[i]);
					}
                }
            }
        }

        if(holdable)
        {
            bool isHeld = trigger[i]->held(c);

            if(   (!heldTrigger || heldTrigger == trigger[i])
               && isHeld
               && (   trigger[i]->isHoldEverywhere()
			       || (isStateSet(STATE_SELECTED | STATE_OVER, c->chan) && trigger[i]->isSelectionClickEverywhere())
				   || this->isInside(c->data.x, c->data.y)
				  )
			  )
            {
                heldTrigger = trigger[i];

                if(!isStateSet(STATE_HELD, c->chan))
                    setState(STATE_HELD, c->chan);

				//! when held more than heldLimit, rumble and send a button event
				if (!pressedLock && ((OSGetTime() / MILLISECS_TO_TICKS(1)) - heldStart) > heldLimit && abs(heldStartXPos - getOffsetX()) < 20 && abs(heldStartYPos - getOffsetY()) < 20)
				{
					//! rumble
					if (rumbleLenght > 0)
						VPADControlMotor(0, rumblePattern, rumbleLenght);

					pressedLong(this, c, trigger[i]);
					
					//! prevent sending pressedLong events until the button is released
					pressedLock = 1;
				}
            }
            else if(isStateSet(STATE_HELD, c->chan) && (heldTrigger == trigger[i]) && (!isHeld || trigger[i]->released(c)))
            {
                //! click is removed at this point and converted to held
                if(clickedTrigger == trigger[i])
                {
                    clickedTrigger = NULL;
                    clearState(STATE_CLICKED, c->chan);
                }
                heldTrigger = NULL;
                clearState(STATE_HELD, c->chan);
				
				//! if the button was held more than the limit it already sent a pressedLong event
				if (((OSGetTime() / MILLISECS_TO_TICKS(1)) - heldStart) <= heldLimit && abs(heldStartXPos - getOffsetX()) < 20 && abs(heldStartYPos - getOffsetY()) < 20)
                {
					//! play the sound after the button is released
				    if(soundClick)
                        soundClick->Play();
					
					//! send the pressed event
					pressed(this, c, trigger[i]);
				}
            }
        }
    }
}
