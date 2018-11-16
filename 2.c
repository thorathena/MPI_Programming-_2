#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {

/*	printf("Enter matrix:\n");
		for(i=0;i<n;i++)
		{
			for(j=0;j<i;j++)
			{
				scanf("%d",&a[i][j]);
			}
				
		}
		printf("\nEnter b vector:\n");
		for(i=0;i<n;i++)
		{
			scanf("%d",&b[i]);
			
		}
 */   
	MPI_Init(NULL, NULL);
	int world_rank,world_size;

	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int n = world_size;
	int a[n][n],b[n],x[n];
	int i,j,sum;
	
	

	/*for(i=0;i<world_size;i++)
    {
	b[i] = rand()%10;
    }
    for(i=0;i<world_size;i++)
    {
	for(j=0;j<i;j++)
	{
		a[i][j] = rand()%10;
	}
    }*/
	if(world_rank == 0)
	{
	printf("Enter matrix:\n");
	}
		for(i=0;i<n;i++)
		{
			for(j=0;j<n;j++)
			{
				if(world_rank == 0)
				{
					scanf("%d",&a[i][j]);
					
				}
				MPI_Bcast(&a[i][j], 1, MPI_INT, 0, MPI_COMM_WORLD);
			}
				
		}
	if(world_rank == 0)
	{
		printf("\nEnter b vector:\n");
	}
		for(i=0;i<n;i++)
		{
			if(world_rank == 0)
			{
				
			scanf("%d",&b[i]);
			}
			
			MPI_Bcast(&b[i], 1, MPI_INT, 0, MPI_COMM_WORLD);
		}

	if(world_rank == 0)
	{
		
		
		//Send broadcast of a matrix and  b vector with tag val=1
		x[0]=b[0]/a[0][0];
		printf("x[0] = %d\n",x[0]);
		MPI_Send(&x[0],1,MPI_INT,1,0,MPI_COMM_WORLD);

	}
	else
	if(world_rank > 0 && world_rank < (world_size-1)) 
	{
	//Send broadcast of a matrix and  b vector with tag val=1
		sum = 0;
		i=world_rank ;
		for(j=0;j<i;j++)
		{
			MPI_Recv(&x[j],1,MPI_INT,i-1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			MPI_Send(&x[j],1,MPI_INT,i+1,0,MPI_COMM_WORLD);
			sum = sum + a[i][j]*x[j];	

			
			
		}
		x[i] = (b[i] - sum)/(a[i][i]*1);
		printf("x[%d] = %d\n",world_rank,x[world_rank]);
		MPI_Send(&x[i],1,MPI_INT,i+1,0,MPI_COMM_WORLD);
		
	}
	else
    {
	for(j=0;j<world_rank;j++)
	{
		MPI_Recv(&x[j], 1, MPI_INT, world_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		sum = sum + a[world_rank][j]*x[j];
	}
	x[world_rank] = (b[world_rank] - sum)/(a[world_rank][world_rank]*1.0);
	printf("x[%d] = %d\n",world_rank,x[world_rank]);
    }
	MPI_Finalize();
}
