/*�ڴ����ר�ÿ�*/
/*Author:bugzhao*/
BYTE **alloc_2d_byt(int m, int n);

/*** ����1ά˫����ʵ������ ***/

double *alloc_1d_dbl(int n)
{
	double *new1;

	new1 = (double *) malloc ((unsigned) (n * sizeof (double)));
	if (new1 == NULL) {
		printf("ALLOC_1D_DBL: Couldn't allocate array of doubles\n");
		return (NULL);
	}
	return (new1);
}

/*** ����2ά˫����ʵ������ ***/

double **alloc_2d_dbl(int m, int n)
{
	int i;
	double **new1;

	new1 = (double **) malloc ((unsigned) (m * sizeof (double *)));
	if (new1 == NULL) {
		printf("ALLOC_2D_DBL: Couldn't allocate array of dbl ptrs\n");
		return (NULL);
	}

	for (i = 0; i < m; i++) {
		new1[i] = alloc_1d_dbl(n);
	}

	return (new1);
}

/*** ����1άBYTE������ ***/

BYTE *alloc_1d_byt(int n)
{
	BYTE *new1;

	new1 = (BYTE *) malloc ((unsigned) (n * sizeof (BYTE)));
	if (new1 == NULL) {
		printf("ALLOC_1D_DBL: Couldn't allocate array of BYTEs\n");
		return (NULL);
	}
	return (new1);
}

/*** ����2άBYTE������ ***/

BYTE **alloc_2d_byt(int m, int n)
{
	int i;
	BYTE **new1;

	new1 = (BYTE **) malloc ((unsigned) (m * sizeof (BYTE *)));
	if (new1 == NULL) {
		printf("ALLOC_2D_DBL: Couldn't allocate array of dbl ptrs\n");
		return (NULL);
	}

	for (i = 0; i < m; i++) {
		new1[i] = alloc_1d_byt(n);
	}

	return (new1);
}