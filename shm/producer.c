#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <semaphore.h>


struct cijfer_t{
	int waarde;
	char* uitspraak;	
};

int main()
{
	int  shm_fd = -1;
	char* sharedmemoryadress;
	const char* shmname = "mysharedmemory4";
	struct cijfer_t* dataptr[10];
	struct cijfer_t cijfers[] = {{.waarde = 0, .uitspraak = "null"},
								{ .waarde = 1, .uitspraak ="een"},
								{ .waarde =2, .uitspraak="twee"},
								{ .waarde =3, .uitspraak="drie"},
								{ .waarde =4, .uitspraak="vier"},
								{ .waarde =5, .uitspraak="vijf"},
								{ .waarde =6, .uitspraak="zes"},
								{ .waarde =7, .uitspraak="zeven"},
								{ .waarde =8, .uitspraak="acht"},
								{ .waarde =9, .uitspraak="negen"}
								};
	int size = sizeof(struct cijfer_t);
	sem_t* lezen = sem_open("/lezen1",O_CREAT,0777, 1);
	// making a shared memory handle.
	printf ("Creating the following shared memory handle: ('%s')\n", shmname);
	shm_fd = shm_open (shmname, O_CREAT | O_EXCL | O_RDWR, 0600);
	
	if (shm_fd == -1)
    {
        perror ("ERROR: shm_open() failed. Exiting now.");
        return -1;
    }
    
    // making the memory the same size as the cijfer_t struct.
    int check = ftruncate (shm_fd, size);
    if(check != 0)
    {
		perror("ERROR: truncate() failed. Exiting now.");
		return -1;
	}
	
	// allocating memory.
	sharedmemoryadress = (char *) mmap (NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (sharedmemoryadress == MAP_FAILED)
    {
        perror ("ERROR: mmap() failed. Exiting now.");
        return -1;
    }
    
    // make 10 spaces for numbers
	for(int i =0; i<10; i++){
		if ((dataptr[i] = (struct cijfer_t*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)) == MAP_FAILED){
			perror("cannot mmap");
			return -1;
		}
	}
	
	// ready to write
	int x = 0;
	while(x < 100)	// for now we don't want to run it forever
	{
		for(int i =0; i <10; i++)
		{	
			
			dataptr[i]->waarde = cijfers[i].waarde;
			dataptr[i]->uitspraak = cijfers[i].uitspraak;
			x++;
			sem_wait(lezen);
		}
	}
	
	munmap(dataptr, size);
    close(shm_fd);
    shm_unlink(shmname);
}
