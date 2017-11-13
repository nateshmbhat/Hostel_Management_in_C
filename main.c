#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>

struct address
{
	char housename[100];
	char city[100];
	char street[100];
};

typedef struct student_structure
{
	char name[100];
	char usn[100];
	struct address addr;
	int prefered_room;  //THIS TAKES THE FIRST PREFERENCE IN ALLOTING ROOMS
	int prefered_floor; // THIS TAKES THE SECOND PREFERENCE IN ALLOTING ROOMS
	struct student_structure *next;
	bool flag_alloted;
} student;

typedef struct rooms
{
	student s1, s2;
	int room_no;
	int room_floor;
	bool flag_taken;
	struct rooms *next;
} ROOM;

typedef struct student_queue
{
	student stu;
	struct student_queue *next;
} STUDENT_QUEUE;




///Prints the string given with a smooth transition
void print_animated(char *str  , int ref)
{
	int wait_time = 0; 
	switch(ref)
	{
		case 1 : wait_time = 2000  ; break ; 
		case 2 : wait_time = 4000 ; break ; 
		case 3: wait_time = 8000  ; break ; 
		case 4 : wait_time = 20000 ; break ; 
		case 5 : wait_time = 40000 ; break ;
	}
	int i = 0;
	for (i = 0; str[i]; i++)
	{
		printf("%c", str[i]);
		usleep(wait_time );
		fflush(stdout);
	}
	fflush(stdout);
	fflush(stdin);
}



///This function is used to add or insert rooms into the pool of rooms and takes the room_no and its floor as input
///Takes any room node as the input and within the function initialises the room_no and floor and flag para
///This is used whenever newrooms are built and have to be added to the list of rooms

///If the room parameter is null then the details of the room are read the file and then the added details are appended to the file .

void Add_room(ROOM *room)
{
	ROOM *newroom = (ROOM *)malloc(sizeof(ROOM));

	printf("Enter the room number : ");
	scanf("%d", &newroom->room_no);
	printf("Enter the floor in which the room exists : ");
	scanf("%d", &newroom->room_floor);
	newroom->flag_taken = false;

	if (room == NULL)
	{
		///Write the new added room to the room_details . THis file contains each room number and its corresponding floor

		FILE *fp_room = fopen("rooms_initial.txt", "a+");
		if (!fp_room)
		{
			printf("\nError opening the rooms_initial.txt file ! \n");
			return;
		}

		fprintf(fp_room, "%d %d\n", newroom->room_no, newroom->room_floor);
		fclose(fp_room);

		print_animated("\nRoom details written successfully to the file ! \n" , 2);
	}

	else
	{

		while (room->next)
			room = room->next;
		room->next = newroom;
	}
	getchar() ;
}

///Reads the rooms details from the file and inserts the rooms into a linked list .
ROOM *read_all_rooms_from_file() ///or insert rooms at the end of list
{

	FILE *fp_room = fopen("rooms_initial.txt", "r+");
	if (!fp_room)
	{
		printf("\nError opening the file rooms_initial.txt !!!\n");
		return 0;
	}

	int ref_flag = 1;
	ROOM *first_room = 0;
	while (!feof(fp_room))
	{
		ROOM *newroom = (ROOM *)malloc(sizeof(ROOM));

		newroom->flag_taken = false;
		newroom->next = 0;
		fscanf(fp_room, "%d %d\n", &newroom->room_no, &newroom->room_floor);

		if (!first_room)
		{
			first_room = newroom;
		}
		else
		{
			ROOM *temp = first_room;
			for (; temp->next; temp = temp->next)
				;
			temp->next = newroom;
		}
	}

	fclose(fp_room);

	return first_room;
}

void Display_inital_room_details(ROOM *first_room)
{
	if (!first_room)
	{
		printf("\nNo Rooms have been added to the Room list yet ");
	}

	else
	{
		printf("\nThe total Room Details are : \n\n");
		for (; first_room; first_room = first_room->next)
		{
			printf("Room Number : %d\nRoom Floor :%d\n\n", first_room->room_no, first_room->room_floor);
		}
	}
}

///returns pointer to first student in the queue
STUDENT_QUEUE *add_student_to_queue(STUDENT_QUEUE *first, student s)
{

	STUDENT_QUEUE *newnode = (STUDENT_QUEUE *)malloc(sizeof(STUDENT_QUEUE));
	if (!newnode)
	{
		printf("\nMaximum memory reached . Malloc failed to allocate memory for a new queue node ! \n");
	}
	newnode->next = NULL;
	newnode->stu = s;

	STUDENT_QUEUE *temp = first;
	if (first == NULL)
	{
		first = newnode;
	}

	else
	{
		while (temp->next)
			temp = temp->next;

		temp->next = newnode;
	}

	// print_animated("\nStudent with below details is added to the allotment queue. " , 2);
	char msg[250];
	sprintf(msg, "\nName : %s\nUSN : %s\n\n", newnode->stu.name, newnode->stu.usn);
	print_animated(msg , 3);
	return first;
}

STUDENT_QUEUE *remove_student_from_queue(STUDENT_QUEUE *first) ///DELETE FIRST STUDENT from queue
{
	if (!first)
	{
		printf("\nThe queue is Empty !\n");
		return NULL;
	}

	STUDENT_QUEUE *temp = first;
	first = first->next;

	print_animated("\n\nStudent with below details is removed from the allotment queue : \n" , 2);
	char msg[250];
	sprintf(msg, "\nName : %s \nUSN : %s \n\n", temp->stu.name, temp->stu.usn);
	print_animated(msg , 3);

	free(temp);
	return first;
}

bool check_if_room_exists(ROOM *first_room, int room_no)
{
	while (first_room->next)
	{
		if (first_room->room_no == room_no)
			return 1;
		first_room = first_room->next;
	}
	return 0;
}

void Display_students_in_queue(STUDENT_QUEUE *first)
{
	system("clear");
	if (first == NULL)
	{
		print_animated("\nThere are currently no students in the allotment queue ! \n" , 3);
		return;
	}
	int i = 1;
	print_animated("\nDetails of Students currently in the Allotment Queue : " , 3);
	print_animated("\n-----------------------------------------------------\n\n" , 3);
	char msg[250];

	for (STUDENT_QUEUE *temp = first; temp != NULL; temp = temp->next)
	{
		sprintf(msg, "\nSl_No: %d \nName : %s\nUSN : %s\n", i, temp->stu.name, temp->stu.usn);
		print_animated(msg , 3);
	}
}

int check_alnum(char *s)
{
	for (int i = 0; s[i]; i++)
		if (!isalnum(s[i]))
			return 0;
	return 1;
}

///THIs takes the first node addr of student struct and returns the first node after inserting the details as last node .
student *register_student(student *first_stu)
{
	system("clear");

	student *newnode = (student *)malloc(sizeof(student));
	newnode->next = 0;

	print_animated("\nEnter the details for the below given fields : \n" , 2);

	char msg[100];
	print_animated("\nName : " , 2);
	gets(newnode->name);
	print_animated("USN : " , 2);
	gets(newnode->usn);
	print_animated("\t\t\tAddress : " , 2);
	print_animated("\nCity : " , 2);
	gets(newnode->addr.city);
	print_animated("Street : " , 2);
	gets(newnode->addr.street);
	print_animated("House_Name : " , 2);
	gets(newnode->addr.housename);

	print_animated("\nPreferred Room number : " , 2);
	scanf("%d", &newnode->prefered_room);
	print_animated("Preferred Floor : " , 2);
	scanf("%d", &newnode->prefered_floor);

	while (getchar() != '\n')
		;

	if (first_stu == NULL)
	{
		first_stu = newnode;
	}

	else
	{
		student *temp;
		for (temp = first_stu; temp->next; temp = temp->next)
			;
		temp->next = newnode;
	}

	FILE *fp = fopen("all_students.txt", "a+");
	setlinebuf(fp);
	fprintf(fp, "\n---\n%s\n%s\n%s\n%s\n%s\n%d\n%d\n\n", newnode->name, newnode->usn, newnode->addr.city, newnode->addr.street, newnode->addr.housename, newnode->prefered_room, newnode->prefered_floor);
	fclose(fp);
	return first_stu;
}

///READS THE STUDENT DETAILS FROM all_students.txt file and inserts them all into a data structure.
student *read_all_students_from_file(student *first_student)
{
	FILE *fp = fopen("all_students.txt", "r");
	if (fp == NULL)
	{
		printf("Error opening file !");
	}

	char line[100];
	int num;
	student *temp;

	setlinebuf(fp);

	while (!feof(fp))
	{
		fgets(line, 100, fp);
		///"---" signifies the start of each student details in the file
		if (!strcmp(line, "---\n"))
		{
			student *newnode = (student *)malloc(sizeof(student));
			fgets(line, 100, fp);
			strtok(line, "\n");
			strcpy(newnode->name, line);

			fgets(line, 100, fp);
			strtok(line, "\n");
			strcpy(newnode->usn, line);

			fgets(line, 100, fp);
			strtok(line, "\n");
			strcpy(newnode->addr.city, line);

			fgets(line, 100, fp);
			strtok(line, "\n");
			strcpy(newnode->addr.street, line);

			fgets(line, 100, fp);
			strtok(line, "\n");
			strcpy(newnode->addr.housename, line);

			fscanf(fp, "%d", &num);
			newnode->prefered_room = num;

			fscanf(fp, "%d", &num);
			newnode->prefered_floor = num;

			newnode->next = 0;

			if (first_student == NULL)
			{
				first_student = newnode;
			}
			else
			{
				for (temp = first_student; temp->next; temp = temp->next)
					;
				temp->next = newnode;
			}
		}
	}

	fclose(fp);
	return first_student;
}

///Displays the student details and takes any student node as param and if howmany is 0 , all details untill null node is printed . Otherwise only specified number of students are printed .
void Display_student_details(student *first, int howmany)
{
	system("clear");
	if (!first)
	{
		printf("\nNo students have been registered yet . Please Register students to view the details ! ");
		return;
	}

	printf("\nStudent Details :- \n");
	printf("-------------------\n");
	student *temp = first;
	int i = 0;
	char msg[250];
	for (; temp && (i < howmany || howmany == 0); temp = temp->next)
	{

		sprintf(msg, "\n--------------------------\n\nName : %s\nUSN : %s\n\t\tAddress :-\nCity : %s\nStreet : %s \nHouse name : %s\n\n--------------------------", temp->name, temp->usn, temp->addr.city, temp->addr.street, temp->addr.housename);
		print_animated(msg , 3);
	}

	print_animated("\n#########" , 2);
}

void Display_alloted_room_details(ROOM *first_room)
{
	system("clear");
	if (!first_room)
	{
		printf("\nThere are currently no rooms in the Default Room list . Please add them \n");
		return;
	}

	FILE *fp_allot = fopen("alloted_room_details" , "w") ;
	if(!fp_allot){
		printf("\nOpening of alloted_room_details failed ! ") ;
	}
	//    printf("\nThe Details of Alloted rooms are : \n") ;

	int flag_not_found = 1;
	for (; first_room; first_room = first_room->next)
	{
		student *s1 = &first_room->s1;
		student *s2 = &first_room->s2;

		// if ((s2->name[0] && s2->name[1]) || (s1->name[0] && s1->name[1]))
		// {
		flag_not_found = 0;
		print_animated("\n\n\n______________________________________________________________________" , 2);
		fprintf(fp_allot ,"\n\n\n______________________________________________________________________");
		printf("\n\n\t\t\tRoom Number : %d", first_room->room_no);
		fprintf(fp_allot , "\n\n\t\t\tRoom Number : %d", first_room->room_no);
		printf("\n\t\t\tRoom Floor  : %d", first_room->room_floor);
		fprintf(fp_allot , "\n\t\t\tRoom Floor  : %d", first_room->room_floor);
		if (first_room->s1.name[0])
		{
			print_animated("\n| Occupant 1 Details : |\n" , 2);
			fprintf(fp_allot  , "\n| Occupant 1 Details : |\n");

			printf("\nName : %s\nUSN: %s \nCity : %s", s1->name, s1->usn, s1->addr.city);
			fprintf(fp_allot , "\nName : %s\nUSN: %s \nCity : %s", s1->name, s1->usn, s1->addr.city);
		}

		else
		{
			print_animated("\n\n\t\t\tThis room is vacant :( " , 2);
			fprintf(fp_allot , "\n\n\t\t\tThis room is vacant :( ");
		}

		if (first_room->s2.name[0])
		{
			print_animated("\n\n\n| Occupant 2 Details : |\n" , 2);
			fprintf(fp_allot ,"\n\n\n| Occupant 2 Details : |\n");
			printf("\nName : %s\nUSN: %s \nCity : %s", s2->name, s2->usn, s2->addr.city);
			fprintf(fp_allot , "\nName : %s\nUSN: %s \nCity : %s", s2->name, s2->usn, s2->addr.city);
		}

		print_animated("\n______________________________________________________________________" , 2);
		fprintf(fp_allot , "\n_____________________________________________________________________");

	}

	fclose(fp_allot) ;
	if (flag_not_found)
	{
		print_animated("\n\nNo Room has been alloted yet !" , 3);
	}
}





STUDENT_QUEUE *add_paid_students_to_student_queue(student *first)
{

	STUDENT_QUEUE *first_queue = 0;

	if (!first)
	{
		printf("\nThere are no students in the LIST OF STUDENTS. Please Register New Students : ");
		return first;
	}

	FILE *fp_paid = fopen("bill_paid_students.txt", "r");

	if (fp_paid == NULL)
	{
		printf("\n Error Opening file !!!");
	}

	//	flag_first_push_to_queue = true ;
	while (!feof(fp_paid))
	{
		bool skip_flag = false;
		char line[100];
		student *stu_temp;
		fgets(line, 100, fp_paid);
		if (feof(fp_paid))
			break;
		strtok(line, "\n");
		for (int i = 0; line[i]; i++)
			if (!isalnum(line[i]))
			{
				skip_flag = true;
			}

		if (skip_flag == false)
		{
			char usn[100];
			for (stu_temp = first; stu_temp; stu_temp = stu_temp->next)
			{
				strcpy(usn, stu_temp->usn);

				if (!strcmp(usn, line))
				{

					first_queue = add_student_to_queue(first_queue, (*stu_temp));
					//					STUDENT_QUEUE *newnode = (STUDENT_QUEUE *)malloc(sizeof(STUDENT_QUEUE));
					//					newnode->stu = (*stu_temp);
					//					newnode->next = 0;
					//
					//					if (first_queue == 0)
					//						first_queue = newnode;
					//
					//					else
					//					{
					//						STUDENT_QUEUE *temp;
					//						for (temp = first_queue; temp->next; temp = temp->next)
					//							;
					//						temp->next = newnode;
					//					}

					break;
				}
			}
		}
	}

	return first_queue;
	//		STUDENT_QUEUE * newnode = (STUDENT_QUEUE *)malloc(sizeof(STUDENT_QUEUE)) ;
}

ROOM *allot_rooms_to_students(STUDENT_QUEUE *que_first, ROOM *room_first)
{
	STUDENT_QUEUE *temp_que = que_first;
	ROOM *temp_room = room_first;
	int pref_room, pref_floor;

	if (!temp_que || !temp_room)
	{
		printf("\nThere are no students right now in the queue OR There are rooms in the ROOM list ! ");
		return room_first;
	}

	///ALLOT BASED ON Prefered ROOM !
	for (temp_que = que_first; temp_que; temp_que = temp_que->next)
	{
		for (temp_room = room_first; temp_room; temp_room = temp_room->next)
		{
			pref_room = temp_que->stu.prefered_room;

			if (pref_room == temp_room->room_no && temp_room->flag_taken != true)
			{
				if (temp_room->s1.usn[0] && temp_room->s1.usn[1])
				{
					temp_room->s2 = (temp_que->stu);
					temp_que->stu.flag_alloted = true;
					temp_room->flag_taken = true;
				}
				else if (temp_room->s2.usn[0] && temp_room->s2.usn[1])
				{
					temp_room->s1 = (temp_que->stu);
					temp_que->stu.flag_alloted = true;
					temp_room->flag_taken = true;
				}
				else
				{
					temp_room->s1 = (temp_que->stu);
					temp_que->stu.flag_alloted = true;
				}
				break;
			}
		}
	}

	///ALLOT ROOMS BASED ON PREFERRED FLOOR
	for (temp_que = que_first; temp_que; temp_que = temp_que->next)
	{
		if (temp_que->stu.flag_alloted)
			continue;

		for (temp_room = room_first; temp_room; temp_room = temp_room->next)
		{
			pref_floor = temp_que->stu.prefered_floor;

			if (pref_floor = temp_room->room_floor && temp_room->flag_taken != true)
			{
				if (temp_room->s1.usn[0] && temp_room->s1.usn[1])
				{
					temp_room->s2 = (temp_que->stu);
					temp_que->stu.flag_alloted = true;
					temp_room->flag_taken = true;
				}
				else if (temp_room->s2.usn[0] && temp_room->s2.usn[1])
				{
					temp_room->s1 = (temp_que->stu);
					temp_que->stu.flag_alloted = true;
					temp_room->flag_taken = true;
				}
				else
				{
					temp_room->s1 = (temp_que->stu);
					temp_que->stu.flag_alloted = true;
				}

				break;
			}
		}
	}

	///ALlotent of the rest of the students who haven't got the desired rooms
	for (temp_que = que_first; temp_que; temp_que = temp_que->next)
	{
		if (temp_que->stu.flag_alloted)
			continue;

		for (temp_room = room_first; temp_room; temp_room = temp_room->next)
		{

			if (temp_room->flag_taken != true)
			{
				if (temp_room->s1.usn[0] && temp_room->s1.usn[1])
				{
					temp_room->s2 = (temp_que->stu);
					temp_que->stu.flag_alloted = true;
					temp_room->flag_taken = true;
				}
				else if (temp_room->s2.usn[0] && temp_room->s2.usn[1])
				{
					temp_room->s1 = (temp_que->stu);
					temp_que->stu.flag_alloted = true;
					temp_room->flag_taken = true;
				}
				else
				{
					temp_room->s1 = (temp_que->stu);
					temp_que->stu.flag_alloted = true;
				}

				break;
			}
		}
	}

	return room_first;
}




void mailsend()
{
    char mystring[2500] ;
        FILE* myfile ;
        myfile = fopen("mailer.py","w") ;
        sprintf(mystring,"\n\
import smtplib\n\
import getpass\n\
with open(\"alloted_room_details\",\"r\") as f :\n\
    message=f.read();\n\
message=\"\"\"Subject:\"Data_Structures\"\n\n\"\"\"+message\n\
try:\n\
    sobj=smtplib.SMTP(\"smtp.gmail.com\",25)\n\
    sobj.starttls()\n\
    sobj.login(\"nateshmbhat1@gmail.com\",getpass.getpass(\"Enter Natesh's gmail password : \")) \n\
    sobj.sendmail(\"nateshmbhat1@gmail.com\",raw_input(\"Enter recepient mail address : \"),message)\n\
    print(\"Mail sent successfully ! \")\n\
except:\n\
    print(\"Internet connection Error ! Check network connection !\")\n\
    ") ;
        fprintf(myfile,mystring) ;
        fclose(myfile) ;
        system("python mailer.py") ;
        system("rm mailer.py") ;
}


void display_paid_students(student *first)
{

}

int main()
{

	ROOM *first_room = 0;
	STUDENT_QUEUE *first_student_queue = 0;
	student *first_student = 0;
	int ch;

	first_room = read_all_rooms_from_file();

	first_student = read_all_students_from_file(first_student);

	getchar();
	int flag_first_time = 1 , wait_time = 4;
	while (1)
	{
		if(flag_first_time)wait_time = 5 ;
		else wait_time = 2 ;
		flag_first_time = 0 ;
		system("clear");
		print_animated("\t\t\tHostel Room Allotment\n" , wait_time);
		print_animated("\t\t\t---------------------\n" , wait_time);

		print_animated("\n1. Register a New Student." , wait_time);
		print_animated("\n2. Display All Student Details. " , wait_time);
		print_animated("\n3. Display Students who have paid the bill." , wait_time) ;
		print_animated("\n4. Add New Room to the available Pool." , wait_time);
		print_animated("\n5. Show the default Room Details." , wait_time);
		print_animated("\n6. Show Alloted Room Details." , wait_time);
		print_animated("\n7. START ALLOTMENT PROCESS." , wait_time);
		print_animated("\n8. Send the Allotment Results as an E-mail." , wait_time) ;

		printf("\n\nEnter choice : ");

		scanf("%d", &ch);
		getchar();
 
		switch (ch)
		{

		case 1:
			first_student = register_student(first_student);
			break;
		case 2:
			Display_student_details(first_student, 0);
			break;
		
		case 3:
			system("clear") ;
			first_student_queue = add_paid_students_to_student_queue(first_student);
			break ; 
		case 4:
			Add_room(0);
			break;

		case 5:
			Display_inital_room_details(first_room);
			break;

		case 6:
			Display_alloted_room_details(first_room);
			break;
		case 7:
			first_student_queue = add_paid_students_to_student_queue(first_student);
			first_room = allot_rooms_to_students(first_student_queue, first_room);
			Display_alloted_room_details(first_room);
			break ; 
		case 8:
			mailsend() ;
			break ; 

		default:
			printf("\n\nInvalid CHOICE !") ;
			

		}

		printf("\n\n\n\nPress Enter to continue : \n");
		getchar();
	}
}
