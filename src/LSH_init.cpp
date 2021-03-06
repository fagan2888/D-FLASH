#include "LSH.h"
#ifndef UINT_MAX
#define UINT_MAX 0xffffffff
#endif
#define RANDPROJGROUPSIZE 100

LSH::LSH(int _K_in, int _L_in, int _rangePow_in, int worldSize, int worldRank)
{

	// Constant Parameters accross all nodes
	_K = _K_in;
	_L = _L_in;
	_numTables = _L_in;
	_rangePow = _rangePow_in;
	_numhashes = _K * _L;
	_lognumhash = log2(_numhashes);

	_rand1 = new int[_K * _L];
	_randHash = new int[2];
	
	//Rancom hash functions
	//srand(time(NULL));
	
	//Fixed random seed for hash functions
	srand(145297);

	// MPI

	_worldSize = worldSize;
	_worldRank = worldRank;

	if (_worldRank == 0)
	{
		for (int i = 0; i < _numhashes; i++)
		{
			_rand1[i] = rand();
			if (_rand1[i] % 2 == 0)
				_rand1[i]++;
		}

		// _randa and _randHash* are random odd numbers.
		_randa = rand();
		if (_randa % 2 == 0)
			_randa++;
		_randHash[0] = rand();
		if (_randHash[0] % 2 == 0)
			_randHash[0]++;
		_randHash[1] = rand();
		if (_randHash[1] % 2 == 0)
			_randHash[1]++;
	}

	MPI_Bcast(&_randa, 1, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Bcast(_randHash, 2, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Bcast(_rand1, _numhashes, MPI_INT, 0, MPI_COMM_WORLD);

	std::cout << "LSH Initialized in Node " << _worldRank << std::endl;
}

LSH::~LSH()
{
	delete[] _randHash;
	delete[] _rand1;
}
