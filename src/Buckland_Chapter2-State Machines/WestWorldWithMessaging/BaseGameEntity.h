#ifndef ENTITY_H
#define ENTITY_H
//------------------------------------------------------------------------
//
//  Name:   BaseGameEntity.h
//
//  Desc:   Base class for a game object
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <string>
#include <vector>

#include "messaging/Telegram.h"


class BaseGameEntity
{

private:

  //every entity must have a unique identifying number
  int          m_ID;

  //this is the next valid ID. Each time a BaseGameEntity is instantiated
  //this value is updated
  static int  m_iNextValidID;

  //this must be called within the constructor to make sure the ID is set
  //correctly. It verifies that the value passed to the method is greater
  //or equal to the next valid ID, before setting the ID and incrementing
  //the next valid ID
  void SetID(int val);

public:

  BaseGameEntity(int id)
  {
    SetID(id);
  }

  virtual ~BaseGameEntity(){}

  //all entities must implement an update function
  virtual void  Update()=0;

  //all entities can communicate using messages. They are sent
  //using the MessageDispatcher singleton class
  virtual bool  HandleMessage(const Telegram& msg)=0;

  /// <summary>
  /// Get all states for an entity.
  /// </summary>
  /// <returns>List of states name available by the current entity.</returns>
  virtual std::vector<std::string> GetStatesNames() = 0;

  /// <summary>
  /// Set initial state from user input.
  /// </summary>
  /// <param name="initialState">Initial state index</param>
  /// <returns>Successful state change</returns>
  virtual bool SetInitialState(int initialState) { return false; }

  int           ID()const{return m_ID;}  
};



#endif