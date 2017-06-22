#include <stdio.h>
#include "tester.h"

const int FanParamCount = 6;
const int FanIterCount = 4000;

static double roundp( const double x )
{
	return( floor( x * 100000000.0 + 0.5 ) / 100000000.0 );
}

double BestScore = 1e30;
double BestItAvg;
double BestItRtAvg;

class CFanOpt : public CBiteOpt
{
public:
	CFanOpt()
	{
		updateDims( FanParamCount );
	}

	virtual void getMinValues( double* const p ) const
	{
		p[ 0 ] = 0.0;
		p[ 1 ] = 0.1;
		p[ 2 ] = 0.0;
		p[ 3 ] = 0.3;
		p[ 4 ] = 0.0;
		p[ 5 ] = 3.0;
	}

	virtual void getMaxValues( double* const p ) const
	{
		p[ 0 ] = 4.0;
		p[ 1 ] = 1.0;
		p[ 2 ] = 1.0;
		p[ 3 ] = 2.9;
		p[ 4 ] = 1.4;
		p[ 5 ] = 12.0;
	}

	virtual double optcost( const double* const p ) const
	{
		CTester Tester;
		Tester.opt -> CostMult = roundp( p[ 0 ]);
		Tester.opt -> MinpMult = roundp( p[ 1 ]);
		Tester.opt -> MaxpMult = roundp( p[ 2 ]);
		Tester.opt -> CentMult = roundp( p[ 3 ]);
		Tester.opt -> CentOffs = roundp( p[ 4 ]);
		Tester.opt -> RandMult = roundp( p[ 5 ]);
		rnd.init( 0 );
//		Tester.init( 2, OptCorpus2D, 0.001, 5000, 10000, true, false );
//		Tester.init( 10, OptCorpusND, 0.01, 150, 20000, false, false );
		Tester.init( 2, TestCorpusAll, 0.000001, 400, 2000, false, false );

		Tester.run();

		if( BestScore > 1e20 || Tester.Score < BestScore )
		{
			BestScore = Tester.Score;
			BestItAvg = Tester.ItAvg;
			BestItRtAvg = Tester.ItRtAvg;
		}

		return( Tester.Score );
	}
};

int main()
{
	CBiteRnd rnd2;
	rnd2.init( 1 );

	double Params[ FanParamCount ];
	Params[ 0 ] = 0.46077270;
	Params[ 1 ] = 0.66309999;
	Params[ 2 ] = 0.50491756;
	Params[ 3 ] = 1.10704201;
	Params[ 4 ] = 0.62276709;
	Params[ 5 ] = 8.04600239;

	CFanOpt opt;
	opt.init( rnd2, Params );
	int i;

	for( i = 0; i < FanIterCount; i++ )
	{
		opt.optimize( rnd2 );

		printf( "// Cost=%f ItAvg=%f ItRtAvg=%f\n", opt.getBestCost(),
			BestItAvg, BestItRtAvg );

		int j;

		for( j = 0; j < FanParamCount; j++ )
		{
			Params[ j ] = roundp( opt.getBestParams()[ j ]);
		}

		printf( "CostMult = %.8f;\n", Params[ 0 ]);
		printf( "MinpMult = %.8f;\n", Params[ 1 ]);
		printf( "MaxpMult = %.8f;\n", Params[ 2 ]);
		printf( "CentMult = %.8f;\n", Params[ 3 ]);
		printf( "CentOffs = %.8f;\n", Params[ 4 ]);
		printf( "RandMult = %.8f;\n", Params[ 5 ]);
	}

	return( 0 );
}
