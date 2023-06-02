// Copyright (C) 2023  CEA/DEN, EDF R&D
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#include "HeatMarcel.hxx"

#include "baseutilities.h"

#include <thread>
#include <cstdint>
#include <cmath>
#include <vector>
#include <numeric>
#include <chrono>

#include <iostream>

template<class T>
static void GetSlice(T start, T stop, const unsigned int sliceId, const unsigned int nbOfSlices, T& startSlice, T& stopSlice)
{
  T nbElems=stop - start;
  T minNbOfElemsPerSlice=nbElems/nbOfSlices;
  startSlice=start+minNbOfElemsPerSlice*sliceId;
  if(sliceId<nbOfSlices-1)
    stopSlice=start+minNbOfElemsPerSlice*(sliceId+1);
  else
    stopSlice=stop;
}

/*!
 * Compute integration of 1/(1+x^2) function (atan function) between start/nbTurn and end/nbTurn.
 */
static void SimulateOneCoreOfComputationNode(std::uint64_t start, std::uint64_t end, std::uint64_t nbTurn, long double* ret)
{
  long double retLoc = 0.0;
  long double delta = 1.0 / ((long double) nbTurn);
  for(std::uint64_t i = start ; i < end ; ++i)
  {
    long double x = i*delta;
    retLoc += delta/(1+x*x);
  }
  *ret = retLoc;
}

static long double SimulateOnAllCoresOfComputationNodeInternal(std::uint64_t nbTurn, unsigned int nbThreads)
{
  SIMPLE_MESSAGE( "Number of turn = 10**" << std::log10((double)nbTurn) );
  std::vector< std::thread > threads(nbThreads);
  std::vector<long double> res(nbThreads);
  for(auto iCore = 0 ; iCore < nbThreads ; ++iCore)
  {
    std::uint64_t startSlice,stopSlice;
    GetSlice<std::uint64_t>(0,nbTurn,iCore,nbThreads,startSlice,stopSlice);
    long double *resLoc = res.data()+iCore;
    threads[iCore] = std::thread([nbTurn,startSlice,stopSlice,resLoc]{ SimulateOneCoreOfComputationNode(startSlice,stopSlice,nbTurn,resLoc);});
  }
  SIMPLE_MESSAGE( "Number of processors " << nbThreads );
  for(auto& th : threads)
    th.join();
  long double ret = std::accumulate(res.begin(),res.end(),0.0);
  return ret;
}

constexpr long double CST = 161000000; // CST - estabished with gcc8.5.0 in debug mode

static std::uint64_t GetNbTurnFromTimeControler(double timeControler)
{
  return static_cast<std::uint64_t>( timeControler * CST );
}

/*!
* long double& ret let it in output to force computation
 */
static std::int64_t GetNbOfNsFor(double timeControler, long double& ret)
{
  std::uint64_t nbTurn = GetNbTurnFromTimeControler( timeControler );
  auto start = std::chrono::high_resolution_clock::now();
  SimulateOneCoreOfComputationNode(0,nbTurn,nbTurn,&ret);
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

/*!
* long double& ret let it in output to force computation
 */
static double FunctionToSolve(double timeControler, long double& fake)
{
  std::int64_t timeInNs = GetNbOfNsFor(timeControler,fake);
  long double ret = (long double )timeInNs / ((long double) 1000000000);
  return ret;
}

/*!
* long double& ret let it in output to force computation
 */
static double Derivative(double timeControler, long double& fake)
{
  long double fake1,fake2;
  double a = FunctionToSolve(timeControler,fake1);
  double b = FunctionToSolve(2*timeControler,fake2);
  fake = fake1 + fake2;
  return (b-a)/(timeControler);
}

/*
 * The returned value is those to pass to ActiveSleepOfComputationNode to
 * have an active sleep of 1s on one core.
 * 
 * Higher is the returned value most performant is your processor and/or the generated binary
 * 
 * Some exemple on core i7-6700 of 2020 on gcc 6.3 and 8.5
 * In debug mode : ~ 1
 * In O2 mode : ~ 4.3
 */
double SALOME::GetTimeAdjustmentCst()
{
  long double fake;
  double ret = Derivative(0.5,fake);
  // Do not remove fake variable because if you do so compiler is too smart and stop computing :)
  if(fake > 0.0)
    return 1.0/ret;
  return 1.0;
}

/*
 * This method can be seen as a active sleep that occupies cores (piloted by \a nbThreads) of hosting computation node.
 * Of course the time spend by this method cannot be controled finely due to dependancy of the activity on hosted machine / OS.
 * This method only occupies cores without any stress of the memory.
 * This method can be used to assess good usage of computation nodes on a massive YACS graph.
 * 
 * /param [in] timeAjustment : pilot time spend for the active sleep. The duration is linearly controled by this input and inversly linearly to the \a nbThreads parameter.
 * \param [out] timeInS : return the time in second of the active sleep
 * \param [in] nbThreads : Specify the number of thread on which the computation will be launched. If 0 (the default) all cores of hosting machine will be used. 
 * \return is pi value computed in a naive way :) This return value is necessary to prevent aggressive compiler to not compute.
 * 
 * \sa GetTimeAdjustmentCstOnYourSystem : to help you to fine tune \a timeAjustment parameter
 */
long double SALOME::HeatMarcel(double timeAjustment, double& timeInS, unsigned int nbThreads)
{
  std::uint64_t nbTurn = GetNbTurnFromTimeControler( timeAjustment );
  unsigned int nbThreadsEff = nbThreads == 0?std::thread::hardware_concurrency():nbThreads;
  auto start = std::chrono::high_resolution_clock::now();
  long double ret = SimulateOnAllCoresOfComputationNodeInternal(nbTurn, nbThreadsEff);
  auto end = std::chrono::high_resolution_clock::now();
  timeInS = ((double)(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()))/1000000000.0;
  return 4.0 * ret;
}
