#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>


struct address
{
	char housename[100]  ;
	char city[100] ;
	char street[100] ;
} ;

typedef struct student
{
	char name[100]  ; 
	int hostel_id ; 
	char usn[100] ;
	struct address addr  ;
	int prefered_room ;
	int prefered_floor ;
}student ;


typedef struct rooms
{
	student s1 , s2 ;
	int room_no ;
	int room_floor;
	bool flag_taken  ;
}ROOM ;


///This function is used to add or insert rooms into the pool of rooms and takes the room_no and its floor as input 
///Takes any room node as the input and within the function initialises the room_no and floor and flag para

void Add_room(ROOM *room)
{
	ROOM *newroom = (ROOM *)malloc(sizeof(ROOM)) ;

	printf("Enter the room number : ") ; 
	scanf("%d" , newroom->room_no) ;
	printf("Enter the floor in which the room exists : ") ; 
	scanf("%d" , newroom->room_floor) ;
	
	if(room==NULL)
	{

		

	}

	else
	{


	}


}


node * add_student_to_queue(node * first )
{



}



int main()
{

}