#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include<unistd.h>

struct address
{
	char housename[100]  ;
	char city[100] ;
	char street[100] ;
} ;

typedef struct student_structure
{
	char name[100]  ;
	int hostel_id ;
	char usn[100] ;
	struct address addr  ;
	int prefered_room ;  //THIS TAKES THE FIRST PREFERENCE IN ALLOTING ROOMS
	int prefered_floor ; // THIS TAKES THE SECOND PREFERENCE IN ALLOTING ROOMS
	struct student_structure *next ;
}student ;



typedef struct rooms
{
	student s1 , s2 ;
	int room_no ;
	int room_floor;
	bool flag_taken  ;
	struct rooms *next ;
}ROOM ;



typedef struct student_queue
{
	student stu ;
	struct student_queue *next ;
}STUDENT_QUEUE ;



///Prints the string given with a smooth transition
void print_animated(char * str)
{
	printf("\n") ;
	int i =0 ;
	for(i=0 ; str[i] ; i++)
	{
		printf("%c" , str[i]) ;
		usleep(70000)  ;
		fflush(stdout) ;
	}
}



///This function is used to add or insert rooms into the pool of rooms and takes the room_no and its floor as input
///Takes any room node as the input and within the function initialises the room_no and floor and flag para
///This is used whenever newrooms are built and have to be added to the list of rooms

///If the room parameter is null then the details of the room are read the file and then the added details are appended to the file .

void Add_room(ROOM *room)
{
	ROOM *newroom = (ROOM *)malloc(sizeof(ROOM)) ;

	printf("Enter the room number : ") ;
	scanf("%d" , newroom->room_no) ;
	printf("Enter the floor in which the room exists : ") ;
	scanf("%d" , newroom->room_floor) ;
	newroom->flag_taken = false ;


	if(room==NULL)
	{
		///Write the new added room to the room_details . THis file contains each room number and its corresponding floor

		FILE *fp_room = fopen("rooms_initial.txt" , "a+");
		if(!fp_room)
		{
			printf("\nError opening the rooms_initial.txt file ! \n" );
			return  ;
		}

		fprintf(fp_room , "%d %d\n" , newroom->room_no , newroom->room_floor);
		fclose(fp_room) ;

		print_animated("\nRoom details written successfully to the file ! \n") ;

	}

	else
	{

		while(room->next)
			room = room->next ;
		room->next = newroom ;

	}
}



///Reads the rooms details from the file and inserts the rooms into a linked list .
ROOM * read_all_rooms_from_file() ///or insert rooms at the end of list
{


	FILE *fp_room = fopen("rooms_initial.txt" , "r+") ;
	if(!fp_room)
	{
		printf("\nError opening the file rooms_initial.txt !!!\n") ;
		return 0 ;
	}

int ref_flag = 1 ;
ROOM *first_room ;
while(!feof(fp_room))
	{
		ROOM *newroom = (ROOM *)malloc(sizeof(ROOM)) ;

		newroom->flag_taken = false ;

		fscanf(fp_room , "%d %d\n" , newroom->room_no , newroom->room_floor) ;

		if(ref_flag){first_room = newroom ; ref_flag = 0 ; }

	}

fclose(fp_room) ;
print_animated("\nInitial Room details have been read successfully into a List ! ") ;

return first_room ;

}






///returns pointer to first student in the queue
STUDENT_QUEUE * add_student_to_queue(STUDENT_QUEUE *first , student s)
{

	STUDENT_QUEUE *newnode = (STUDENT_QUEUE *)malloc(sizeof(STUDENT_QUEUE)) ;
	if(!newnode)
	{
		printf("\nMaximum memory reached . Malloc failed to allocate memory for a new queue node ! \n") ;
	}
	newnode->next = NULL ;
	newnode->stu = s ;

	STUDENT_QUEUE *temp = first ;
	if(first == NULL)
	{
		first = newnode ;
	}

	else
	{
		while(temp->next)
			temp = temp->next ;

		temp->next = newnode ;
	}

	print_animated("\nStudent with below details is added to the allotment queue. ") ;
	char msg[250]  ;
	sprintf(msg , "\nName : %s\nUSN : %s\n\n" , newnode->stu.name , newnode->stu.usn) ;
	print_animated(msg) ;
	return first ;
}




STUDENT_QUEUE * remove_student_from_queue(STUDENT_QUEUE *first) ///DELETE FIRST STUDENT from queue
{

	if(!first)
	{
		printf("\nThe queue is Empty !\n") ;
		return NULL ;
	}

	STUDENT_QUEUE * temp = first ;
	first = first->next ;

	print_animated("\n\nStudent with below details is removed from the allotment queue : \n") ;
	char msg[250] ;
	sprintf(msg ,"\nName : %s \nUSN : %s \n\n" , temp->stu.name , temp->stu.usn) ;
	print_animated(msg) ;

	free(temp) ;
	return first ;
}



bool check_if_room_exists(ROOM *first_room , int room_no)
{
	while(first_room->next)
	{
		if(first_room->room_no ==room_no)
			return 1 ;
		first_room = first_room->next ;
	}
	return 0 ;
}


void Display_students_in_queue(STUDENT_QUEUE * first)
{
    if(first==NULL)
    {
		print_animated("\nThere are currently no students in the allotment queue ! \n") ;
		return ;
    }
	int i = 1 ;
    print_animated("\nDetails of Students currently in the Allotment Queue : ") ;
    print_animated("\n-----------------------------------------------------\n\n") ;
    char msg[250] ;

    for(STUDENT_QUEUE *temp= first ; temp!=NULL ; temp=temp->next)
    {
		sprintf(msg ,"\nSl_No: %d \nName : %s\nUSN : %s\n" , i , temp->stu.name , temp->stu.usn) ;
		print_animated(msg) ;
    }
}


int main()
{

	ROOM *first_room = 0 ;
	STUDENT_QUEUE *first_student_queue = 0  ;

	printf("\t\t\tHostel Room Allotment") ;
	printf("\t\t\t---------------------\n") ;

}
