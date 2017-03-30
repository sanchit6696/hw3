#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

int main( int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Function needs ring size as input arguments!\n");
        abort();
    }
    int N=atol(argv[1]);
    
    int rank, tag, count,i;
    MPI_Status status;
    
    char hostname[1024];
    gethostname(hostname, 1024);
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);
    
    int *message_in;
    int *message_out;
    tag = 99;
    for(i=0;i<N;i++){
        if(rank!=0)
        {
            MPI_Recv(message_in,  1, MPI_INT, rank-1, tag, MPI_COMM_WORLD, &status);
            message_out=message_in;
        }
        else{
            if(i==0)
                message_out=(int*)calloc(2048, sizeof(int));;
            
        }
        MPI_Send(&message_out, 1, MPI_INT, (rank+1)%count, tag, MPI_COMM_WORLD);
        
        if(rank==0)
        {
            MPI_Recv(&message_in, 1, MPI_INT, count - 1, tag, MPI_COMM_WORLD,&status);
        }
    }
    
    printf("rank %d hosted on %s received the message %d\n", rank, hostname, message_out);
    free(message_in);
    free(message_out);
    MPI_Finalize();
    return 0;
    
}
