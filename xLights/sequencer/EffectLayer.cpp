#include "EffectLayer.h"
#include <algorithm>
#include <vector>
#include "EffectsGrid.h"
#include "Effect.h"
#include "RowHeading.h"
#include "ModelClass.h"


int EffectLayer::exclusive_index = 0;

EffectLayer::EffectLayer(Element* parent)
{
    mParentElement = parent;
    mIndex = exclusive_index++;
}

EffectLayer::~EffectLayer()
{
    for (int x = 0; x < mEffects.size(); x++) {
        delete mEffects[x];
    }
}

int EffectLayer::GetIndex()
{
    return mIndex;
}

Effect* EffectLayer::GetEffect(int index)
{
    if(index < mEffects.size())
    {
        return mEffects[index];
    }
    else
    {
        return nullptr;
    }
}

Effect* EffectLayer::GetEffectFromID(int id)
{
    Effect* eff = nullptr;
    for (int x = 0; x < mEffects.size(); x++) {
        if( mEffects[x]->GetID() == id )
        {
            eff = mEffects[x];
            break;
        }
    }
    return eff;
}

void EffectLayer::RemoveEffect(int index)
{
    if(index<mEffects.size())
    {
        Effect *e = mEffects[index];
        mEffects.erase(mEffects.begin()+index);
        IncrementChangeCount(e->GetStartTime() * 1000, e->GetEndTime() * 1000);
        delete e;
    }
}


Effect* EffectLayer::AddEffect(int id, const wxString &name, const wxString &settings, const wxString &palette,
                               double startTime,double endTime, int Selected, bool Protected)
{
    return AddEffect(id, Effect::GetEffectIndex(name), name, settings, palette, startTime, endTime, Selected, Protected);
}
Effect* EffectLayer::AddEffect(int id, int effectIndex, const wxString &name, const wxString &settings, const wxString &palette,
                               double startTime, double endTime, int Selected, bool Protected)
{
    Effect *e = new Effect(this, id, effectIndex, name, settings, palette, startTime, endTime, Selected, Protected);
    mEffects.push_back(e);
    SortEffects();
    IncrementChangeCount(startTime * 1000, endTime * 1000);
    return e;
}


void EffectLayer::SortEffects()
{
    std::sort(mEffects.begin(),mEffects.end(),SortEffectByStartTime);
    for (int x = 0; x < mEffects.size(); x++) {
        mEffects[x]->SetID(x);
    }
}

bool EffectLayer::IsStartTimeLinked(int index)
{
    if(index < mEffects.size() && index > 0)
    {
        return mEffects[index-1]->GetEndTime() == mEffects[index]->GetStartTime();
    }
    else
    {
        return false;
    }
}

bool EffectLayer::IsEndTimeLinked(int index)
{
    if(index < mEffects.size()-1)
    {
        return mEffects[index]->GetEndTime() == mEffects[index+1]->GetStartTime();
    }
    else
    {
        return false;
    }
}

float EffectLayer::GetMaximumEndTime(int index)
{
    if(index+1 >= mEffects.size())
    {
        return NO_MIN_MAX_TIME;
    }
    else
    {
        if(mEffects[index]->GetEndTime() == mEffects[index+1]->GetStartTime())
        {
            return mEffects[index+1]->GetEndTime();
        }
        else
        {
            return mEffects[index+1]->GetStartTime();
        }
    }
}

float EffectLayer::GetMinimumStartTime(int index)
{
    if(index == 0)
    {
        return NO_MIN_MAX_TIME;
    }
    else
    {
        if(mEffects[index-1]->GetEndTime() == mEffects[index]->GetStartTime())
        {
            return mEffects[index-1]->GetStartTime();
        }
        else
        {
            return mEffects[index-1]->GetEndTime();
        }
    }
}

int EffectLayer::GetEffectCount()
{
    if(mEffects.empty())
        return 0;
    else
        return mEffects.size();
}

bool EffectLayer::HitTestEffect(int position,int &index, int &result)
{
    bool isHit=false;
    for(int i=0;i<mEffects.size();i++)
    {
        if (position >= mEffects[i]->GetStartPosition() &&
            position <= mEffects[i]->GetEndPosition())
        {
            index = i;
            if(position < mEffects[i]->GetStartPosition() + 5)
            {
                isHit = true;
                result = HIT_TEST_EFFECT_LT;
            }
            else if(position > mEffects[i]->GetEndPosition() - 5)
            {
                isHit = true;
                result = HIT_TEST_EFFECT_RT;
            }
            else
            {
                int midpoint = mEffects[i]->GetStartPosition() + (mEffects[i]->GetEndPosition()-mEffects[i]->GetStartPosition())/2;
                if( std::abs(position - midpoint) <= 7 )
                {
                    isHit = true;
                    result = HIT_TEST_EFFECT_CTR;
                }
            }
            break;
        }
    }
    return isHit;
}

bool EffectLayer::HitTestEffectByTime(double time,int &index)
{
    bool isHit=false;
    for(int i=0;i<mEffects.size();i++)
    {
        if (time >= mEffects[i]->GetStartTime() &&
            time <= mEffects[i]->GetEndTime())
        {
            isHit = true;
            index = i;
            break;
        }
    }
    return isHit;
}

int EffectLayer::GetEffectIndexThatContainsPosition(int position,int &selectionType)
{
   int index=-1;
   selectionType = EFFECT_NOT_SELECTED;
    for(int i=0;i<mEffects.size();i++)
    {
        if (position >= mEffects[i]->GetStartPosition() &&
            position <= mEffects[i]->GetEndPosition())
        {
            index = i;

            if(position < mEffects[i]->GetStartPosition() + 5)
            {
                selectionType = EFFECT_LT_SELECTED;
            }
            else if(position > mEffects[i]->GetEndPosition() - 5)
            {
                selectionType = EFFECT_RT_SELECTED;
            }
            else
            {
                selectionType = EFFECT_SELECTED;
            }
            break;
        }
    }
    return index;
}

Effect* EffectLayer::GetEffectBeforePosition(int position)
{
    int i;
    for(i=0; i<mEffects.size();i++)
    {
        if(mEffects[i]->GetStartPosition()>position)
        {
            break;
        }
    }
    if(i==0)
    {
        return nullptr;
    }
    else
    {
        return mEffects[i-1];
    }
}

Effect* EffectLayer::GetEffectAfterPosition(int position)
{
    int i;
    for(i=0; i<mEffects.size();i++)
    {
        if(mEffects[i]->GetEndPosition()<position)
        {
            break;
        }
    }
    if(i==mEffects.size()-1)
    {
        return nullptr;
    }
    else
    {
        return mEffects[i+1];
    }
}

Effect*  EffectLayer::GetEffectBeforeEmptySpace(int position)
{
    int i;
    for(i=mEffects.size()-1; i >= 0; i--)
    {
        if( mEffects[i]->GetEndPosition() < position )
        {
            break;
        }
    }
    if(i<0)
    {
        return nullptr;
    }
    else
    {
        return mEffects[i];
    }
}

Effect*  EffectLayer::GetEffectAfterEmptySpace(int position)
{
    int i;
    for(i=0; i < mEffects.size(); i++)
    {
        if( mEffects[i]->GetStartPosition() > position )
        {
            break;
        }
    }
    if(i==mEffects.size())
    {
        return nullptr;
    }
    else
    {
        return mEffects[i];
    }
}

bool EffectLayer::GetRangeIsClear(int startX, int endX)
{
    int i;
    for(i=0; i<mEffects.size();i++)
    {
        // check if start is between effect range
        if( (startX > mEffects[i]->GetStartPosition()) && (startX < mEffects[i]->GetEndPosition()) )
        {
            return false;
        }
        // check if end is between effect range
        if( (endX > mEffects[i]->GetStartPosition()) && (endX < mEffects[i]->GetEndPosition()) )
        {
            return false;
        }
        // check effect is between start and end
        if( (mEffects[i]->GetStartPosition() >= startX) && (mEffects[i]->GetEndPosition() <= endX) )
        {
            return false;
        }
    }
    return true;
}

bool EffectLayer::GetRangeIsClear(double start_time, double end_time)
{
    int i;
    for(i=0; i<mEffects.size();i++)
    {
        // check if start is between effect range
        if( (start_time > mEffects[i]->GetStartTime()) && (start_time < mEffects[i]->GetEndTime()) )
        {
            return false;
        }
        // check if end is between effect range
        if( (end_time > mEffects[i]->GetStartTime()) && (end_time < mEffects[i]->GetEndTime()) )
        {
            return false;
        }
        // check effect is between start and end
        if( (mEffects[i]->GetStartTime() >= start_time) && (mEffects[i]->GetEndTime() <= end_time) )
        {
            return false;
        }
    }
    return true;
}



void EffectLayer::SelectEffectsInPositionRange(int startX,int endX)
{
    for(int i=0;i<mEffects.size();i++)
    {
        if(mEffects[i]->GetStartPosition() < 0 &&
           mEffects[i]->GetEndPosition() < 0)
        {
            continue;
        }
        int center = mEffects[i]->GetStartPosition() + ((mEffects[i]->GetEndPosition() - mEffects[i]->GetStartPosition())/2);
        int squareWidth =  center<DEFAULT_ROW_HEADING_HEIGHT-6?MINIMUM_EFFECT_WIDTH_FOR_SMALL_RECT:DEFAULT_ROW_HEADING_HEIGHT-6;
        int squareLeft = center - (squareWidth/2);
        int squareRight = center + (squareWidth/2);
        // If selection around icon/square
        if (startX>squareLeft && endX < squareRight)
        {
            if(mEffects[i]->GetSelected()==EFFECT_NOT_SELECTED)
            {
                mEffects[i]->SetSelected(EFFECT_SELECTED);
            }
        }
        else if (startX<squareLeft && endX > squareRight)
        {
            if(mEffects[i]->GetSelected()==EFFECT_NOT_SELECTED)
            {
                mEffects[i]->SetSelected(EFFECT_SELECTED);
            }
        }
        // If selection on left side
        else if (endX>mEffects[i]->GetStartPosition() && endX<squareLeft)
        {
            if(mEffects[i]->GetSelected()==EFFECT_NOT_SELECTED)
            {
                mEffects[i]->SetSelected(EFFECT_LT_SELECTED);
            }
        }
        // If selection on right side
        else if (startX > squareRight && startX < mEffects[i]->GetEndPosition())
        {
            if(mEffects[i]->GetSelected()==EFFECT_NOT_SELECTED)
            {
                mEffects[i]->SetSelected(EFFECT_RT_SELECTED);
            }
        }
    }
}
bool EffectLayer::HasEffectsInTimeRange(double startTime,double endTime) {
    for(int i=0;i<mEffects.size();i++)
    {
        if(mEffects[i]->GetStartTime() >= startTime &&  mEffects[i]->GetStartTime() < endTime)
        {
            return true;
        }
        else if(mEffects[i]->GetEndTime() <= endTime &&  mEffects[i]->GetEndTime() > startTime)
        {
            return true;
        }
    }
    return false;
}

int EffectLayer::SelectEffectsInTimeRange(double startTime,double endTime)
{
    int num_selected = 0;
    for(int i=0;i<mEffects.size();i++)
    {
        if(mEffects[i]->GetStartTime() >= startTime &&  mEffects[i]->GetStartTime() < endTime)
        {
            mEffects[i]->SetSelected(EFFECT_SELECTED);
            num_selected++;
        }
        else if(mEffects[i]->GetEndTime() <= endTime &&  mEffects[i]->GetEndTime() > startTime)
        {
            mEffects[i]->SetSelected(EFFECT_SELECTED);
            num_selected++;
        }
    }
    return num_selected;
}


void EffectLayer::UnSelectAllEffects()
{
    for(int i=0;i<mEffects.size();i++)
    {
        mEffects[i]->SetSelected(EFFECT_NOT_SELECTED);
    }
}

Element* EffectLayer::GetParentElement()
{
    return mParentElement;

}

void EffectLayer::SetParentElement(Element* parent)
{
    mParentElement = parent;
}

void EffectLayer::GetMaximumRangeOfMovementForSelectedEffects(double &toLeft,double &toRight)
{
    toLeft = NO_MAX;
    toRight = NO_MAX;
    for(int i=0;i<mEffects.size();i++)
    {
        if(mEffects[i]->GetSelected() != EFFECT_NOT_SELECTED)
        {
            double l,r;
            GetMaximumRangeOfMovementForEffect(i,l,r);
            toLeft = toLeft<l?toLeft:l;
            toRight = toRight<r?toRight:r;
        }
    }
}

void EffectLayer::GetMaximumRangeOfMovementForEffect(int index, double &toLeft, double &toRight)
{
    switch(mEffects[index]->GetSelected())
    {
        case EFFECT_LT_SELECTED:
            GetMaximumRangeWithLeftMovement(index,toLeft,toRight);
            break;
        case EFFECT_RT_SELECTED:
            GetMaximumRangeWithRightMovement(index,toLeft,toRight);
            break;
        case EFFECT_SELECTED:
            double l1,l2,r1,r2;
            GetMaximumRangeWithLeftMovement(index,l1,r1);
            GetMaximumRangeWithRightMovement(index,l2,r2);
            toLeft = l1<l2?l1:l2;
            toRight = r1<r2?r1:r2;
            break;
        default:
            break;
    }
}

void EffectLayer::GetMaximumRangeWithLeftMovement(int index, double &toLeft, double &toRight)
{
    toRight = mEffects[index]->GetEndTime() - mEffects[index]->GetStartTime();
    if(index == 0)
    {
       toLeft = mEffects[index]->GetStartTime();
    }
    else
    {
       if(mEffects[index-1]->GetSelected() == EFFECT_NOT_SELECTED)
       {
          toLeft = mEffects[index]->GetStartTime() - mEffects[index-1]->GetEndTime();
       }
       else if(mEffects[index-1]->GetSelected() == EFFECT_RT_SELECTED)
       {
          toLeft = mEffects[index]->GetStartTime() - mEffects[index-1]->GetStartTime();
       }
       else if(mEffects[index-1]->GetSelected() == EFFECT_SELECTED)
       {
           // Do not know so set to maximum. Let the effects to left decide
          toLeft = mEffects[index]->GetStartTime();
       }
    }
}

void EffectLayer::GetMaximumRangeWithRightMovement(int index, double &toLeft, double &toRight)
{
    toLeft = mEffects[index]->GetEndTime() - mEffects[index]->GetStartTime();
    // Last effect, nothing to right to stop movement other then edge of screen.
    // Let grid take care of screen boundary so set to huge number
    if(index == mEffects.size()-1)
    {
       toRight = NO_MAX;
    }
    else
    {
       if(mEffects[index+1]->GetSelected() == EFFECT_NOT_SELECTED)
       {
          toRight = mEffects[index+1]->GetStartTime() - mEffects[index]->GetEndTime();
       }
       else if(mEffects[index+1]->GetSelected() == EFFECT_LT_SELECTED)
       {
          toRight = mEffects[index+1]->GetEndTime() - mEffects[index]->GetEndTime();
       }
       else if(mEffects[index+1]->GetSelected() == EFFECT_SELECTED)
       {
           // Do not know so set to maximum. Let the effects to right decide
          toRight = NO_MAX;
       }
    }
}

int EffectLayer::GetSelectedEffectCount()
{
    wxString s;
    int count=0;
    for(int i=0; i<mEffects.size();i++)
    {
        if(mEffects[i]->GetSelected() != EFFECT_NOT_SELECTED)
        {
           count++;
        }
    }
    return count;
}

void EffectLayer::UpdateAllSelectedEffects(const wxString& palette)
{
    for(int i=0; i<mEffects.size();i++)
    {
        if(mEffects[i]->GetSelected() != EFFECT_NOT_SELECTED )
        {
            mEffects[i]->SetPalette(palette);
        }
    }
}

void EffectLayer::MoveAllSelectedEffects(double delta, UndoManager& undo_mgr)
{
    for(int i=0; i<mEffects.size();i++)
    {
        if(mEffects[i]->GetSelected() == EFFECT_LT_SELECTED)
        {
            if( undo_mgr.GetCaptureUndo() ) {
                undo_mgr.CaptureEffectToBeMoved( mParentElement->GetName(), mIndex, mEffects[i]->GetID(),
                                                 mEffects[i]->GetStartTime(), mEffects[i]->GetEndTime() );
            }
            mEffects[i]->SetStartTime( mEffects[i]->GetStartTime()+ delta);
        }
        else if(mEffects[i]->GetSelected() == EFFECT_RT_SELECTED)
        {
            if( undo_mgr.GetCaptureUndo() ) {
                undo_mgr.CaptureEffectToBeMoved( mParentElement->GetName(), mIndex, mEffects[i]->GetID(),
                                                 mEffects[i]->GetStartTime(), mEffects[i]->GetEndTime() );
            }
            mEffects[i]->SetEndTime( mEffects[i]->GetEndTime()+ delta);
        }
        else if(mEffects[i]->GetSelected() == EFFECT_SELECTED)
        {
            if( undo_mgr.GetCaptureUndo() ) {
                undo_mgr.CaptureEffectToBeMoved( mParentElement->GetName(), mIndex, mEffects[i]->GetID(),
                                                 mEffects[i]->GetStartTime(), mEffects[i]->GetEndTime() );
            }
            mEffects[i]->SetStartTime( mEffects[i]->GetStartTime()+ delta);
            mEffects[i]->SetEndTime( mEffects[i]->GetEndTime()+ delta);
        }
    }
}

void EffectLayer::DeleteSelectedEffects(UndoManager& undo_mgr)
{
    for (std::vector<Effect*>::iterator it = mEffects.begin(); it != mEffects.end(); it++) {
        if ((*it)->GetSelected() != EFFECT_NOT_SELECTED) {
             IncrementChangeCount((*it)->GetStartTime() * 1000, (*it)->GetEndTime() * 1000);
            undo_mgr.CaptureEffectToBeDeleted( mParentElement->GetName(), mIndex, (*it)->GetEffectName(),
                                               (*it)->GetSettingsAsString(), (*it)->GetPaletteAsString(),
                                               (*it)->GetStartTime(), (*it)->GetEndTime(),
                                               (*it)->GetSelected(), (*it)->GetProtected() );
        }
    }
    mEffects.erase(std::remove_if(mEffects.begin(), mEffects.end(),ShouldDeleteSelected),mEffects.end());
}
void EffectLayer::DeleteEffectByIndex(int idx) {
    mEffects.erase(mEffects.begin()+idx);
}
void EffectLayer::DeleteEffect(int id)
{
    for(int i=0; i<mEffects.size();i++)
    {
        if(mEffects[i]->GetID() == id)
        {
           mEffects.erase(mEffects.begin()+i);
           break;
        }
    }
}

bool EffectLayer::ShouldDeleteSelected(Effect *eff)
{
    return eff->GetSelected() != EFFECT_NOT_SELECTED;
}

bool EffectLayer::SortEffectByStartTime(Effect *e1,Effect *e2)
{
    return e1->GetStartTime() < e2->GetStartTime();
}

void EffectLayer::IncrementChangeCount(int startMS, int endMS)
{
    mParentElement->IncrementChangeCount(startMS, endMS);
    changeCount++;
}


StrandLayer::~StrandLayer() {
    for (int x = 0; x < mNodeLayers.size(); x++) {
        delete mNodeLayers[x];
    }
}

NodeLayer *StrandLayer::GetNodeLayer(int n, bool create) {
    while (create && n >= mNodeLayers.size()) {
        mNodeLayers.push_back(new NodeLayer(GetParentElement(),""));
    }
    if (n < mNodeLayers.size()) {
        return mNodeLayers[n];
    }
    return nullptr;
}
void StrandLayer::InitFromModel(ModelClass &model) {
    int nc = model.GetStrandLength(strand);
    name = model.GetStrandName(strand);
    for (int x = 0; x < mNodeLayers.size(); x++) {
        mNodeLayers[x]->SetName(model.GetNodeName(x));
    }
    while (mNodeLayers.size() < nc) {
        NodeLayer *nl = new NodeLayer(GetParentElement(), model.GetNodeName(mNodeLayers.size()));
        mNodeLayers.push_back(nl);
    }
}
