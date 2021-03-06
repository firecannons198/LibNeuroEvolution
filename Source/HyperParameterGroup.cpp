/*
    Programmer: Lucas Winkelman
    File: HyperParameterGroup.cpp
    Date: 9/22/2017
    Purpose: HyperParameterGroup.cpp contains definitions for the
        HyperParameterGroup class in LibNeuroEvolution.
*/

#include "HyperParameterGroup.h"

namespace LNE
{

    HyperParameterGroup :: HyperParameterGroup( vector < unsigned int > & InSizes , unsigned int InNumberNetworksPerGroup )
    {
        NumberNetworksPerGroup = InNumberNetworksPerGroup ;
        WeightShiftRangeTop = DEFAULT_WEIGHT_SHIFT_RANGE_TOP ;
        WeightShiftRangeBottom = DEFAULT_WEIGHT_SHIFT_RANGE_BOTTOM ;
        NewWeightRangeTop = DEFAULT_NEW_WEIGHT_RANGE_TOP ;
        NewWeightRangeBottom = DEFAULT_NEW_WEIGHT_RANGE_BOTTOM ;
        KillRatio = DEFAULT_KILL_RATIO ;
        MutateRatio = DEFAULT_MUTATE_RATIO ;
        CurrentNetworkIndex = 0 ;
        Networks . resize ( NumberNetworksPerGroup ) ;
        unsigned int NetworkIterator = 0 ;
        while ( NetworkIterator < NumberNetworksPerGroup )
        {
            Networks [ NetworkIterator ] = new NeuralNetwork ( InSizes ) ;
            NetworkIterator = NetworkIterator + 1 ;
        }
    }

    HyperParameterGroup :: ~ HyperParameterGroup ( )
    {
        DeleteNetworks ( ) ;
    }

    HyperParameterGroup :: HyperParameterGroup ( const HyperParameterGroup & SourceGroup )
    {
        NumberNetworksPerGroup = 0;
        * this = SourceGroup ;
    }

    HyperParameterGroup & HyperParameterGroup :: operator = ( const HyperParameterGroup & SourceGroup )
    {
        if ( this == & SourceGroup )
        {
            return * this ;
        }
        else
        {
            DeleteNetworks ( ) ;
            CopyNetworks ( SourceGroup . Networks ) ;
            WeightShiftRangeTop = SourceGroup . WeightShiftRangeTop ;
            WeightShiftRangeBottom = SourceGroup . WeightShiftRangeBottom ;
            NewWeightRangeTop = SourceGroup . NewWeightRangeTop ;
            NewWeightRangeBottom = SourceGroup . NewWeightRangeBottom ;
            KillRatio = SourceGroup . KillRatio ;
            MutateRatio = SourceGroup . MutateRatio ;
            NumberNetworksPerGroup = SourceGroup . NumberNetworksPerGroup ;
            CurrentNetworkIndex = SourceGroup . CurrentNetworkIndex ;

            return * this;
        }
    }

    void HyperParameterGroup :: ToNextNetwork ( )
    {
        CurrentNetworkIndex = CurrentNetworkIndex + 1 ;
    }

    unsigned int HyperParameterGroup :: GetCurrentNetworkIndex ( ) const
    {
        return CurrentNetworkIndex ;
    }

    void HyperParameterGroup :: SortNetworks ( )
    {
        unsigned int NetworkIterator = 0 ;
        while ( NetworkIterator < NumberNetworksPerGroup - 1 )
        {
            unsigned int NetworkIterator2 = NumberNetworksPerGroup - 1 ;
            while ( NetworkIterator2 > NetworkIterator )
            {
                if ( Networks [ NetworkIterator2 ] -> GetFitness ( ) > Networks [ NetworkIterator2 - 1 ] -> GetFitness ( ) )
                {
                    NeuralNetwork * Temp = Networks [ NetworkIterator2 ] ;
                    Networks [ NetworkIterator2 ] = Networks [ NetworkIterator2 - 1 ] ;
                    Networks [ NetworkIterator2 - 1 ] = Temp ;
                }
                NetworkIterator2 = NetworkIterator2 - 1 ;
            }
            NetworkIterator = NetworkIterator + 1 ;
        }
    }

    float HyperParameterGroup :: GetBestNetworkFitness ( ) const
    {
        return Networks [ 0 ] -> GetFitness ( ) ;
    }

    NeuralNetwork * HyperParameterGroup :: GetBestNetwork ( ) const
    {
        return Networks [ 0 ] ;
    }

    bool HyperParameterGroup :: AreNetworksDoneRunning ( ) const
    {
        bool Output = false ;
        if ( CurrentNetworkIndex >= NumberNetworksPerGroup )
        {
            Output = true ;
        }
        return Output ;
    }

    NeuralNetwork * HyperParameterGroup :: GetCurrentNetwork ( ) const
    {
        NeuralNetwork * OutputNetwork ;
        if ( CurrentNetworkIndex < NumberNetworksPerGroup )
        {
            OutputNetwork = Networks [ CurrentNetworkIndex ] ;
        }
        else
        {
            OutputNetwork = NULL ;
        }
        return OutputNetwork ;
    }

    void HyperParameterGroup :: EndGeneration ( )
    {
        SortNetworks ( ) ;
        KillNetworks ( ) ;
        MutateNetworks ( ) ;
        CurrentNetworkIndex = 0 ;
    }

    void HyperParameterGroup :: Mutate ( )
    {
        MutateKillRatio ( ) ;
        MutateMutateRatio ( ) ;
        MutateWeightShiftRangeTop ( ) ;
        MutateWeightShiftRangeBottom ( ) ;
        ResolveWeightShiftRange ( ) ;
        MutateNewWeightRangeTop ( ) ;
        MutateNewWeightRangeBottom ( ) ;
        ResolveNewWeightRange ( ) ;
        MutateWeightShiftChance ( ) ;
        MutateWeightNewChance ( ) ;
    }

    void HyperParameterGroup :: RandomizeValues ( )
    {
        float WeightShiftRangeTopRange = WEIGHT_SHIFT_RANGE_TOP_SHIFT_MAX - WEIGHT_SHIFT_RANGE_TOP_SHIFT_MIN ;
        WeightShiftRangeTop = GetProb ( ) * WeightShiftRangeTopRange + WEIGHT_SHIFT_RANGE_TOP_SHIFT_MIN ;

        float WeightShiftRangeBottomRange = WEIGHT_SHIFT_RANGE_BOTTOM_SHIFT_MAX - WEIGHT_SHIFT_RANGE_BOTTOM_SHIFT_MIN ;
        WeightShiftRangeBottom = GetProb ( ) * WeightShiftRangeBottomRange + WEIGHT_SHIFT_RANGE_BOTTOM_SHIFT_MIN ;

        float NewWeightRangeTopRange = NEW_WEIGHT_RANGE_TOP_NEW_MAX - NEW_WEIGHT_RANGE_TOP_NEW_MIN ;
        NewWeightRangeTop = GetProb ( ) * NewWeightRangeTopRange + NEW_WEIGHT_RANGE_TOP_NEW_MIN ;

        float NewWeightRangeBottomRange = NEW_WEIGHT_RANGE_BOTTOM_NEW_MAX - NEW_WEIGHT_RANGE_BOTTOM_NEW_MIN ;
        NewWeightRangeBottom = GetProb ( ) * NewWeightRangeBottomRange + NEW_WEIGHT_RANGE_BOTTOM_NEW_MIN ;

        float KillRatioRange = KILL_RATIO_NEW_MAX - KILL_RATIO_NEW_MIN ;
        KillRatio = GetProb ( ) * KillRatioRange + KILL_RATIO_NEW_MIN ;

        float MutateRatioRange = MUTATE_RATIO_NEW_MAX - MUTATE_RATIO_NEW_MIN ;
        MutateRatio = GetProb ( ) * MutateRatioRange + MUTATE_RATIO_NEW_MIN ;

        float WeightShiftChanceRange = WEIGHT_SHIFT_CHANCE_NEW_MAX - WEIGHT_SHIFT_CHANCE_NEW_MIN ;
        WeightShiftChance = GetProb ( ) * WeightShiftChanceRange + WEIGHT_SHIFT_CHANCE_NEW_MIN ;

        float WeightNewChanceRange = WEIGHT_NEW_CHANCE_NEW_MAX - WEIGHT_NEW_CHANCE_NEW_MIN ;
        WeightNewChance = GetProb ( ) * WeightNewChanceRange + WEIGHT_NEW_CHANCE_NEW_MIN ;
    }

    void HyperParameterGroup :: MutateWeightNewChance ( )
    {
        float RandomNumber = GetProb ( ) ;
        if ( RandomNumber < WEIGHT_NEW_CHANCE_NEW_CHANCE )
        {
            float WeightNewChanceNewDist = WEIGHT_NEW_CHANCE_NEW_MAX - WEIGHT_NEW_CHANCE_NEW_MIN ;
            float NewValue = GetProb ( ) * WeightNewChanceNewDist + WEIGHT_NEW_CHANCE_NEW_MIN ;
            WeightNewChance = NewValue ;
        }
        RandomNumber = GetProb ( ) ;
        if ( RandomNumber < WEIGHT_NEW_CHANCE_SHIFT_CHANCE )
        {
            float WeightNewChanceShiftDist = WEIGHT_NEW_CHANCE_SHIFT_MAX - WEIGHT_NEW_CHANCE_SHIFT_MIN ;
            float ShiftAmount = GetProb ( ) * WeightNewChanceShiftDist + WEIGHT_NEW_CHANCE_SHIFT_MIN ;
            WeightNewChance = MutateRatio + ShiftAmount ;
        }
        if ( WeightNewChance < WEIGHT_NEW_CHANCE_MAX )
        {
            WeightNewChance = WEIGHT_NEW_CHANCE_MAX ;
        }
        else if ( WeightNewChance > WEIGHT_NEW_CHANCE_MIN )
        {
            WeightNewChance = WEIGHT_NEW_CHANCE_MIN ;
        }
    }

    void HyperParameterGroup :: MutateWeightShiftChance ( )
    {
        float RandomNumber = GetProb ( ) ;
        if ( RandomNumber < WEIGHT_SHIFT_CHANCE_NEW_CHANCE )
        {
            float WeightShiftChanceNewDist = WEIGHT_SHIFT_CHANCE_NEW_MAX - WEIGHT_SHIFT_CHANCE_NEW_MIN ;
            float NewValue = GetProb ( ) * WeightShiftChanceNewDist + WEIGHT_SHIFT_CHANCE_NEW_MIN ;
            WeightShiftChance = NewValue ;
        }
        RandomNumber = GetProb ( ) ;
        if ( RandomNumber < WEIGHT_SHIFT_CHANCE_SHIFT_CHANCE )
        {
            float WeightShiftChanceShiftDist = WEIGHT_SHIFT_CHANCE_SHIFT_MAX - WEIGHT_SHIFT_CHANCE_SHIFT_MIN ;
            float ShiftAmount = GetProb ( ) * WeightShiftChanceShiftDist + WEIGHT_SHIFT_CHANCE_SHIFT_MIN ;
            WeightShiftChance = MutateRatio + ShiftAmount ;
        }
        if ( WeightShiftChance < WEIGHT_SHIFT_CHANCE_MAX )
        {
            WeightShiftChance = WEIGHT_SHIFT_CHANCE_MAX ;
        }
        else if ( WeightShiftChance > WEIGHT_SHIFT_CHANCE_MIN )
        {
            WeightShiftChance = WEIGHT_SHIFT_CHANCE_MIN ;
        }
    }

    void HyperParameterGroup :: ResolveNewWeightRange ( )
    {
        if ( NewWeightRangeTop < NewWeightRangeBottom )
        {
            float Difference = NewWeightRangeBottom - NewWeightRangeTop ;
            NewWeightRangeTop = NewWeightRangeTop + ( Difference / 2 ) ;
            NewWeightRangeBottom = NewWeightRangeBottom - ( Difference / 2 ) ;
        }
    }


    void HyperParameterGroup :: MutateNewWeightRangeBottom ( )
    {
        float RandomNumber = GetProb ( ) ;
        if ( RandomNumber < NEW_WEIGHT_RANGE_BOTTOM_NEW_CHANCE )
        {
            float NewWeightRangeBottomNewDist = NEW_WEIGHT_RANGE_BOTTOM_NEW_MAX - NEW_WEIGHT_RANGE_BOTTOM_NEW_MIN ;
            float NewValue = GetProb ( ) * NewWeightRangeBottomNewDist + NEW_WEIGHT_RANGE_BOTTOM_NEW_MIN ;
            NewWeightRangeBottom = NewValue ;
        }
        RandomNumber = GetProb ( ) ;
        if ( RandomNumber < NEW_WEIGHT_RANGE_BOTTOM_SHIFT_CHANCE )
        {
            float NewWeightRangeBottomShiftDist = NEW_WEIGHT_RANGE_BOTTOM_SHIFT_MAX - NEW_WEIGHT_RANGE_BOTTOM_SHIFT_MIN ;
            float ShiftAmount = GetProb ( ) * NewWeightRangeBottomShiftDist + NEW_WEIGHT_RANGE_BOTTOM_SHIFT_MIN ;
            NewWeightRangeBottom = MutateRatio + ShiftAmount ;
        }
        if ( NewWeightRangeBottom < NEW_WEIGHT_RANGE_BOTTOM_MIN )
        {
            NewWeightRangeBottom = NEW_WEIGHT_RANGE_BOTTOM_MIN ;
        }
        else if ( NewWeightRangeBottom > NEW_WEIGHT_RANGE_BOTTOM_MAX )
        {
            NewWeightRangeBottom = NEW_WEIGHT_RANGE_BOTTOM_MAX ;
        }
    }

    void HyperParameterGroup :: MutateNewWeightRangeTop ( )
    {
        float RandomNumber = GetProb ( ) ;
        if ( RandomNumber < NEW_WEIGHT_RANGE_TOP_NEW_CHANCE )
        {
            float NewWeightRangeTopNewDist = NEW_WEIGHT_RANGE_TOP_NEW_MAX - NEW_WEIGHT_RANGE_TOP_NEW_MIN ;
            float NewValue = GetProb ( ) * NewWeightRangeTopNewDist + NEW_WEIGHT_RANGE_TOP_NEW_MIN ;
            NewWeightRangeTop = NewValue ;
        }
        RandomNumber = GetProb ( ) ;
        if ( RandomNumber < NEW_WEIGHT_RANGE_TOP_SHIFT_CHANCE )
        {
            float NewWeightRangeTopShiftDist = NEW_WEIGHT_RANGE_TOP_SHIFT_MAX - NEW_WEIGHT_RANGE_TOP_SHIFT_MIN ;
            float ShiftAmount = GetProb ( ) * NewWeightRangeTopShiftDist + NEW_WEIGHT_RANGE_TOP_SHIFT_MIN ;
            NewWeightRangeTop = MutateRatio + ShiftAmount ;
        }
        if ( NewWeightRangeTop < NEW_WEIGHT_RANGE_TOP_MIN )
        {
            NewWeightRangeTop = NEW_WEIGHT_RANGE_TOP_MIN ;
        }
        else if ( NewWeightRangeTop > NEW_WEIGHT_RANGE_TOP_MAX )
        {
            NewWeightRangeTop = NEW_WEIGHT_RANGE_TOP_MAX ;
        }
    }

    void HyperParameterGroup :: ResolveWeightShiftRange ( )
    {
        if ( WeightShiftRangeTop < WeightShiftRangeBottom )
        {
            float Difference = WeightShiftRangeBottom - WeightShiftRangeTop ;
            WeightShiftRangeTop = WeightShiftRangeTop + ( Difference / 2 ) ;
            WeightShiftRangeBottom = WeightShiftRangeBottom - ( Difference / 2 ) ;
        }
    }

    void HyperParameterGroup :: MutateWeightShiftRangeTop ( )
    {
        float RandomNumber = GetProb ( ) ;
        if ( RandomNumber < WEIGHT_SHIFT_RANGE_TOP_NEW_CHANCE )
        {
            float WeightShiftRangeTopNewDist = WEIGHT_SHIFT_RANGE_TOP_NEW_MAX - WEIGHT_SHIFT_RANGE_TOP_NEW_MIN ;
            float NewValue = GetProb ( ) * WeightShiftRangeTopNewDist + WEIGHT_SHIFT_RANGE_TOP_NEW_MIN ;
            WeightShiftRangeTop = NewValue ;
        }
        RandomNumber = GetProb ( ) ;
        if ( RandomNumber < WEIGHT_SHIFT_RANGE_TOP_SHIFT_CHANCE )
        {
            float WeightShiftRangeTopShiftDist = WEIGHT_SHIFT_RANGE_TOP_SHIFT_MAX - WEIGHT_SHIFT_RANGE_TOP_SHIFT_MIN ;
            float ShiftAmount = GetProb ( ) * WeightShiftRangeTopShiftDist + WEIGHT_SHIFT_RANGE_TOP_SHIFT_MIN ;
            WeightShiftRangeTop = MutateRatio + ShiftAmount ;
        }
        if ( WeightShiftRangeTop < WEIGHT_SHIFT_RANGE_TOP_MIN )
        {
            WeightShiftRangeTop = WEIGHT_SHIFT_RANGE_TOP_MIN ;
        }
        else if ( WeightShiftRangeTop > WEIGHT_SHIFT_RANGE_TOP_MAX )
        {
            WeightShiftRangeTop = WEIGHT_SHIFT_RANGE_TOP_MAX ;
        }
    }



    void HyperParameterGroup :: MutateWeightShiftRangeBottom ( )
    {
        float RandomNumber = GetProb ( ) ;
        if ( RandomNumber < WEIGHT_SHIFT_RANGE_BOTTOM_NEW_CHANCE )
        {
            float WeightShiftRangeBottomNewDist = WEIGHT_SHIFT_RANGE_BOTTOM_NEW_MAX - WEIGHT_SHIFT_RANGE_BOTTOM_NEW_MIN ;
            float NewValue = GetProb ( ) * WeightShiftRangeBottomNewDist + WEIGHT_SHIFT_RANGE_BOTTOM_NEW_MIN ;
            WeightShiftRangeBottom = NewValue ;
        }
        RandomNumber = GetProb ( ) ;
        if ( RandomNumber < WEIGHT_SHIFT_RANGE_BOTTOM_SHIFT_CHANCE )
        {
            float WeightShiftRangeBottomShiftDist = WEIGHT_SHIFT_RANGE_BOTTOM_SHIFT_MAX - WEIGHT_SHIFT_RANGE_BOTTOM_SHIFT_MIN ;
            float ShiftAmount = GetProb ( ) * WeightShiftRangeBottomShiftDist + WEIGHT_SHIFT_RANGE_BOTTOM_SHIFT_MIN ;
            WeightShiftRangeBottom = MutateRatio + ShiftAmount ;
        }
        if ( WeightShiftRangeBottom < WEIGHT_SHIFT_RANGE_BOTTOM_MIN )
        {
            WeightShiftRangeBottom = WEIGHT_SHIFT_RANGE_BOTTOM_MIN ;
        }
        else if ( WeightShiftRangeBottom > WEIGHT_SHIFT_RANGE_BOTTOM_MAX )
        {
            WeightShiftRangeBottom = WEIGHT_SHIFT_RANGE_BOTTOM_MAX ;
        }
    }


    void HyperParameterGroup :: MutateKillRatio ( )
    {
        float RandomNumber = GetProb ( ) ;
        if ( RandomNumber < KILL_RATIO_NEW_CHANCE )
        {
            float KillRatioNewDist = KILL_RATIO_NEW_MAX - KILL_RATIO_NEW_MIN ;
            float NewKillRatio = GetProb ( ) * KillRatioNewDist + KILL_RATIO_NEW_MIN ;
            KillRatio = NewKillRatio ;
        }
        RandomNumber = GetProb ( ) ;
        if ( RandomNumber < KILL_RATIO_SHIFT_CHANCE )
        {
            float KillRatioShiftDist = KILL_RATIO_SHIFT_MAX - KILL_RATIO_SHIFT_MIN ;
            float ShiftAmount = GetProb ( ) * KillRatioShiftDist + KILL_RATIO_SHIFT_MIN ;
            KillRatio = KillRatio + ShiftAmount ;
        }
        if ( KillRatio < KILL_RATIO_MIN )
        {
            KillRatio = KILL_RATIO_MIN ;
        }
        else if ( KillRatio > KILL_RATIO_MAX )
        {
            KillRatio = KILL_RATIO_MAX ;
        }
    }

    void HyperParameterGroup :: MutateMutateRatio ( )
    {
        float RandomNumber = GetProb ( ) ;
        if ( RandomNumber < MUTATE_RATIO_NEW_CHANCE )
        {
            float MutateRatioNewDist = MUTATE_RATIO_NEW_MAX - MUTATE_RATIO_NEW_MIN ;
            float NewMutateRatio = GetProb ( ) * MutateRatioNewDist + MUTATE_RATIO_NEW_MIN ;
            MutateRatio = NewMutateRatio ;
        }
        RandomNumber = GetProb ( ) ;
        if ( RandomNumber < MUTATE_RATIO_SHIFT_CHANCE )
        {
            float MutateRatioShiftDist = MUTATE_RATIO_SHIFT_MAX - MUTATE_RATIO_SHIFT_MIN ;
            float ShiftAmount = GetProb ( ) * MutateRatioShiftDist + MUTATE_RATIO_SHIFT_MIN ;
            MutateRatio = MutateRatio + ShiftAmount ;
        }
        if ( MutateRatio < MUTATE_RATIO_MIN )
        {
            MutateRatio = MUTATE_RATIO_MIN ;
        }
        else if ( MutateRatio > MUTATE_RATIO_MAX )
        {
            MutateRatio = MUTATE_RATIO_MAX ;
        }
    }



    void HyperParameterGroup :: DeleteNetworks ( )
    {
        unsigned int NetworkIterator = 0 ;
        while ( NetworkIterator < NumberNetworksPerGroup )
        {
            delete Networks [ NetworkIterator ] ;
            NetworkIterator = NetworkIterator + 1 ;
        }
    }

    void HyperParameterGroup :: CopyNetworks ( const vector < NeuralNetwork * > & SourceNetworks )
    {
        Networks . resize ( SourceNetworks . size ( ) ) ;
        unsigned int NetworkIterator = 0 ;
        while ( NetworkIterator < SourceNetworks . size ( ) )
        {
            Networks [ NetworkIterator ] = new NeuralNetwork ( * ( SourceNetworks [ NetworkIterator ] ) ) ;
            NetworkIterator = NetworkIterator + 1 ;
        }
    }

    void HyperParameterGroup :: KillNetworks ( )
    {
        unsigned int StartingKillIndex = ceil ( NumberNetworksPerGroup * ( 1 - KillRatio ) ) ;
        unsigned int NetworkIterator = StartingKillIndex ;
        while ( NetworkIterator < NumberNetworksPerGroup )
        {
            unsigned int SourceNetworkIndex = NetworkIterator - StartingKillIndex ;
            * ( Networks [ NetworkIterator ] ) = * ( Networks [ SourceNetworkIndex ] ) ;
            NetworkIterator = NetworkIterator + 1 ;
        }
    }

    void HyperParameterGroup :: MutateNetworks ( )
    {
        unsigned int StartingMutateIndex = ceil ( NumberNetworksPerGroup * ( 1 - MutateRatio ) ) ;
        unsigned int NetworkIterator = StartingMutateIndex ;
        while ( NetworkIterator < NumberNetworksPerGroup )
        {
            Networks [ NetworkIterator ] -> Mutate ( WeightShiftChance , WeightNewChance , WeightShiftRangeTop , WeightShiftRangeBottom ,
                                                     NewWeightRangeTop , NewWeightRangeBottom ) ;
            NetworkIterator = NetworkIterator + 1 ;
        }
    }

}
