#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_PLACES 35
#define MAX_TRAINS 10
#define MAX_PASSENGERS 100
#define MAX_USERS 100
#define MAX_NAME_LENGTH 50
int pnrcheck=0;
    char *cyan="\033[1:36m";
    char *purple="\033[1;35m";
   char *green = "\033[1;32m";
   char *red="\033[1;31m";
 char *yellow = "\033[1;33m";
    char *reset = "\033[0m";
typedef struct {
    int seat_number;
    int is_booked;
    char preference[20];
    char coach[3];
    double fare;
} Seat;

typedef struct {
    char name[MAX_NAME_LENGTH];
    Seat *seats;
    int total_seats;
    int available_seats;
    char departure_time[10];
    char arrival_time[10];
    char from[MAX_NAME_LENGTH];
    char to[MAX_NAME_LENGTH];
    char type[10]; // "Night" or "Day"
    int train_number;
    char date[11]; // Travel date
} Train;

typedef struct {
    char places[MAX_PLACES][MAX_NAME_LENGTH];
    Train trains[MAX_TRAINS];
} RailwaySystem;
Train trains[MAX_TRAINS] = {
        {"Shatabdi Express", NULL, 0, 0, "09:00", "12:00", "Chennai", "Coimbatore", "Day", 1, ""},
        {"Rajdhani Express", NULL, 0, 0, "09:00", "13:00", "Chennai", "Madurai", "Night", 2, ""},
        {"Duronto Express", NULL, 0, 0, "10:00", "14:00", "Coimbatore", "Madurai", "Night", 3, ""},
        {"Garib Rath", NULL, 0, 0, "19:00", "23:00", "Tiruchirappalli", "Chennai", "Day", 4, ""},
        {"Intercity Express", NULL, 0, 0, "12:00", "16:00", "Madurai", "Chennai", "Day", 5,""},
        {"Jan Shatabdi", NULL, 0, 0, "13:00", "17:00", "Salem", "Chennai", "Day", 6,""},
        {"Chennai Express", NULL, 0, 0, "14:00", "18:00", "Tirunelveli", "Tiruchirappalli", "Day", 7,""},
        {"Coimbatore Express", NULL, 0, 0, "15:00", "19:00", "Madurai", "Coimbatore", "Night", 8,""},
        {"Pandian Express", NULL, 0, 0, "16:00", "20:00", "Chennai", "Madurai", "Night", 9,""},
        {"Pallavan Express", NULL, 0, 0, "17:00", "21:00", "Chennai", "Tiruchirappalli", "Day", 10,""},
        
    };
typedef struct {
    int pnr;
    char passenger_name[MAX_NAME_LENGTH];
    char train_name[MAX_NAME_LENGTH];
    char from[MAX_NAME_LENGTH];
    char to[MAX_NAME_LENGTH];
    char departure1_time[10];
    char arrival1_time[10];
    char date[11];
    int seat_number;
    char coach[3];
    char seat_type[20]; // Seater/Berth
    char username[MAX_NAME_LENGTH]; // Added field to store the username of the passenger
} Ticket;

typedef struct {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    char payment_password[MAX_NAME_LENGTH];
    char security_question[MAX_NAME_LENGTH];
    char security_answer[MAX_NAME_LENGTH];
    int bank_balance;
} User;

Ticket bookings[MAX_PASSENGERS];
Ticket waiting_list[MAX_PASSENGERS];
int booking_count = 0;
int waiting_count = 0;
User users[MAX_USERS];
int user_count = 0;

int is_valid_password(const char *password) {
    if (strlen(password) < 8) return 0;

    int has_upper = 0, has_digit = 0, has_special = 0;
    for (int i = 0; password[i] != '\0'; i++) {
        if (isupper(password[i])) has_upper = 1;
        else if (isdigit(password[i])) has_digit = 1;
        else if (ispunct(password[i])) has_special = 1;
    }
    return has_upper && has_digit && has_special;
}

void load_users() {
    FILE *file = fopen("users.dat", "rb");
    if (file != NULL) {
        fread(&user_count, sizeof(int), 1, file);
        fread(users, sizeof(User), user_count, file);
        fclose(file);
    }
}

void save_users() {
    FILE *file = fopen("users.dat", "wb");
    if (file != NULL) {
        fwrite(&user_count, sizeof(int), 1, file);
        fwrite(users, sizeof(User), user_count, file);
        fclose(file);
    }
}

void load_bookings() {
    FILE *file = fopen("bookings.dat", "rb");
    if (file != NULL) {
        fread(&booking_count, sizeof(int), 1, file);
        fread(bookings, sizeof(Ticket), booking_count, file);
        fclose(file);
    }
}

void save_bookings() {
    FILE *file = fopen("bookings.dat", "wb");
    if (file != NULL) {
        fwrite(&booking_count, sizeof(int), 1, file);
        fwrite(bookings, sizeof(Ticket), booking_count, file);
        fclose(file);
    }
}

void save_waiting_list() {
    FILE *file = fopen("waiting_list.dat", "wb");
    if (file != NULL) {
        fwrite(&waiting_count, sizeof(int), 1, file);
        fwrite(waiting_list, sizeof(Ticket), waiting_count, file);
        fclose(file);
    }
}

void load_waiting_list() {
    FILE *file = fopen("waiting_list.dat", "rb");
    if (file != NULL) {
        fread(&waiting_count, sizeof(int), 1, file);
        fread(waiting_list, sizeof(Ticket), waiting_count, file);
        fclose(file);
    }
}

void save_seat_status(RailwaySystem *system) {
    FILE *file = fopen("seat_status.dat", "wb");
    if (file != NULL) {
        for (int i = 0; i < MAX_TRAINS; i++) {
            fwrite(&system->trains[i].total_seats, sizeof(int), 1, file);
            fwrite(system->trains[i].seats, sizeof(Seat), system->trains[i].total_seats, file);
        }
        fclose(file);
    }
}

void load_seat_status(RailwaySystem *system) {
    FILE *file = fopen("seat_status.dat", "rb");
    if (file != NULL) {
        for (int i = 0; i < MAX_TRAINS; i++) {
            fread(&system->trains[i].total_seats, sizeof(int), 1, file);
            fread(system->trains[i].seats, sizeof(Seat), system->trains[i].total_seats, file);
        }
        fclose(file);
    }
}

void signup() {
    if (user_count >= MAX_USERS) {
        printf("User limit reached. Cannot signup more users.\n");
        return;
    }

    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    char payment_password[MAX_NAME_LENGTH];
    char security_question[MAX_NAME_LENGTH];
    char security_answer[MAX_NAME_LENGTH];

    printf("Enter a username: ");
    scanf("%s", username);

    // Check if username already exists
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("Username already exists. Please choose a different username.\n");
            return;
        }
    }

    while (1) {
        printf("Enter a password (min 8 characters, including uppercase, digit, special): ");
        scanf("%s", password);
        if (is_valid_password(password)) {
            break;
        } else {
            printf("Password does not meet criteria. Try again.\n");
        }
    }

    printf("Enter a payment password (min 8 characters, including uppercase, digit, special): ");
    scanf("%s", payment_password);
    printf("Enter a security question: ");
    getchar(); // to consume newline left by previous input
    fgets(security_question, MAX_NAME_LENGTH, stdin);
    strtok(security_question, "\n"); // remove trailing newline
    printf("Enter the answer to the security question: ");
    fgets(security_answer, MAX_NAME_LENGTH, stdin);
    strtok(security_answer, "\n"); // remove trailing newline

    strcpy(users[user_count].username, username);
    strcpy(users[user_count].password, password);
    strcpy(users[user_count].payment_password, payment_password);
    strcpy(users[user_count].security_question, security_question);
    strcpy(users[user_count].security_answer, security_answer);
    user_count++;

    save_users();

    printf("Signup successful. You can now login.\n");
}

int login() {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];

    printf("Enter your username: ");
    scanf("%s", username);
    printf("Enter your password: ");
    scanf("%s", password);

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            printf("Login successful.\n");
            return i + 1; // Login successful
        }
    }

    printf("Invalid username or password.\n");
    return 0; // Login failed
}

void retrieve_password() {
    char username[MAX_NAME_LENGTH];
    char security_answer[MAX_NAME_LENGTH];

    printf("Enter your username: ");
    scanf("%s", username);

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("Security Question: %s\n", users[i].security_question);
            printf("Enter your answer: ");
            getchar(); // to consume newline left by previous input
            fgets(security_answer, MAX_NAME_LENGTH, stdin);
            strtok(security_answer, "\n"); // remove trailing newline

            if (strcmp(users[i].security_answer, security_answer) == 0) {
                printf("Your password is: %s\n", users[i].password);
                return;
            } else {
                printf("Incorrect answer. Cannot retrieve password.\n");
                return;
            }
        }
    }

    printf("Username not found.\n");
}

void view_ticket() {
    int pnr;
    display_available_pnrs();
    if(pnrcheck==0) return;
    printf("Enter the PNR number: ");
    scanf("%d", &pnr);

    for (int i = 0; i < booking_count; i++) {
        if (bookings[i].pnr == pnr) {
            for(int j=0;j<11;++j){ if(strcmp(bookings[i].train_name,trains[j].name)==0)
            {strcpy(bookings[i].departure1_time,trains[j].departure_time);
             strcpy(bookings[i].arrival1_time,trains[j].arrival_time);}}
           
             printf("\n");
           // ANSI escape code for yellow color
    char *yellow = "\033[1;33m";
    char *reset = "\033[0m";

    // Print the ticket details in yellow
    printf("%s", yellow);
    printf("*********************\n");
    printf("*  Indian Railways  *\n");
    printf("*********************\n");
    printf("*  Ticket Details   *\n");
    printf("*********************\n");
    printf("PASSENGER UNIQUE PNR: %d\n", bookings[i].pnr);   
    printf("Train: %s\n", bookings[i].train_name);
    printf("From: %s\n", bookings[i].from);
    printf("To: %s\n", bookings[i].to);
    printf("Date: %s\n", bookings[i].date);
    printf("Seat: %d\n", bookings[i].seat_number);
    printf("Coach: %s\n", bookings[i].coach);
    printf("Seat Type: %s\n", bookings[i].seat_type);
    printf("Departure Time: %s\n", bookings[i].departure1_time);
    printf("Arrival Time: %s\n", bookings[i].arrival1_time);
    printf("*********************\n");
    printf("%s", reset);

            return;
        }
    }
    printf("PNR not found.\n");
}

int process_payment(double amount, const char* payment_password) {
    char entered_password[MAX_NAME_LENGTH];
    printf("-->Processing payment of Rs.%.2f\n", amount);
    printf("-->Enter payment password: ");
    scanf("%s", entered_password);

    if (strcmp(payment_password, entered_password) == 0) {
        printf("\n-->Payment successful.!!\n");
        return 1;
    } else {
        printf("\n-->Payment failed. Incorrect payment password!!.\n");
        return 0;
    }
}

double calculate_refund_amount(const char *travel_date, const char *cancellation_date, double original_amount) {
    int travel_year, travel_month, travel_day;
    int cancel_year, cancel_month, cancel_day;

    sscanf(travel_date, "%d-%d-%d", &travel_year, &travel_month, &travel_day);
    sscanf(cancellation_date, "%d-%d-%d", &cancel_year, &cancel_month, &cancel_day);

    struct tm travel_tm = {0};
    struct tm cancel_tm = {0};

    travel_tm.tm_year = travel_year - 1900;
    travel_tm.tm_mon = travel_month - 1;
    travel_tm.tm_mday = travel_day;

    cancel_tm.tm_year = cancel_year - 1900;
    cancel_tm.tm_mon = cancel_month - 1;
    cancel_tm.tm_mday = cancel_day;

    time_t travel_time = mktime(&travel_tm);
    time_t cancel_time = mktime(&cancel_tm);
    double diff_days = difftime(travel_time, cancel_time) / (60 * 60 * 24);

    double refund_percentage = 0.0;
    if (diff_days >= 10) {
        refund_percentage = 1.0;
    } else if (diff_days >= 5) {
        refund_percentage = 0.7;
    } else if (diff_days >= 3) {
        refund_percentage = 0.5;
    } else if (diff_days >= 1) {
        refund_percentage = 0.2;
    } else {
        refund_percentage = 0.0;
    }

    return original_amount * refund_percentage;
}

void clear_screen() {
    for (int i = 0; i < 40; i++) {
        printf("\n");
    }
}

void initialize_system(RailwaySystem *system) {

    char places[MAX_PLACES][MAX_NAME_LENGTH] = {
        "Chennai", "Coimbatore", "Madurai", "Tiruchirappalli", "Salem",
        "Tirunelveli", "Tiruppur", "Ranipet", "Erode", "Vellore",
        "Thoothukudi", "Dindigul", "Thanjavur", "Kanchipuram", "Kumarapalayam",
        "Karur", "Cuddalore", "Kumbakonam", "Nagercoil", "Theni",
        "Nagapattinam", "Udhagamandalam", "Pollachi", "Ramanathapuram", "Pudukkottai",
        "Virudhunagar", "Sivakasi", "Viluppuram", "Hosur", "Karaikudi",
        "Sivaganga", "Arakkonam", "Kodaikanal", "Perambalur", "Tindivanam",
        
    };
    for (int i = 0; i < MAX_PLACES; i++) {
        strcpy(system->places[i], places[i]);
    }

   

    for (int i = 0; i < MAX_TRAINS; i++) {
        system->trains[i] = trains[i];

        if (strcmp(system->trains[i].type, "Night") == 0) {
            int total_seats = 12 * 72 + 4 * 64 + 3 * 32 + 1 * 1;
            system->trains[i].seats = (Seat *)malloc(total_seats * sizeof(Seat));
            system->trains[i].total_seats = total_seats;
            system->trains[i].available_seats = total_seats;
            int seat_number = 1;
            int k = 0;
            for (int j = 0; j < 12 * 72; j++, seat_number++) {
                char coach[3] = "S";
                char number[3];
                snprintf(number, sizeof(number), "%d", k / 72 + 1);
                strcat(coach, number);
                system->trains[i].seats[j] = (Seat){seat_number, 0, "Sleeper", "Sleeper", 200.0};
                strcpy(system->trains[i].seats[j].coach, coach);
                k++;
            }
            for (int j = 12 * 72; j < 12 * 72 + 4 * 64; j++, seat_number++) {
                char coach[3] = "B";
                char number[3];
                snprintf(number, sizeof(number), "%d", (k - 12 * 72) / 64 + 1);
                strcat(coach, number);
                system->trains[i].seats[j] = (Seat){seat_number, 0, "AC 3 Tier", "Upper", 500.0};
                strcpy(system->trains[i].seats[j].coach, coach);
                k++;
            }
            for (int j = 12 * 72 + 4 * 64; j < 12 * 72 + 4 * 64 + 3 * 32; j++, seat_number++) {
                char coach[3] = "A";
                char number[3];
                snprintf(number, sizeof(number), "%d", (k - 12 * 72 - 4 * 64) / 32 + 1);
                strcat(coach, number);
                system->trains[i].seats[j] = (Seat){seat_number, 0, "AC 2 Tier", "Lower", 800.0};
                strcpy(system->trains[i].seats[j].coach, coach);
                k++;
            }
            for (int j = 12 * 72 + 4 * 64 + 3 * 32; j < total_seats; j++, seat_number++) {
                char coach[3] = "F";
                snprintf(coach, sizeof(coach), "%d", 1);
                system->trains[i].seats[j] = (Seat){seat_number, 0, "AC First", "Lower", 1000.0};
                strcpy(system->trains[i].seats[j].coach, coach);
            }
        } else if (strcmp(system->trains[i].type, "Day") == 0) {
            int total_seats = 7 * 120 + 5 * 60 + 2 * 45;
            system->trains[i].seats = (Seat *)malloc(total_seats * sizeof(Seat));
            system->trains[i].total_seats = total_seats;
            system->trains[i].available_seats = total_seats;
            int seat_number = 1;
            int k = 0;
            for (int j = 0; j < 7 * 120; j++, seat_number++) {
                char coach[3] = "D";
                char number[3];
                snprintf(number, sizeof(number), "%d", k / 120 + 1);
                strcat(coach, number);
                system->trains[i].seats[j] = (Seat){seat_number, 0, "General", "Aisle", 150.0};
                strcpy(system->trains[i].seats[j].coach, coach);
                k++;
            }
            for (int j = 7 * 120; j < 7 * 120 + 5 * 60; j++, seat_number++) {
                char coach[3] = "C";
                char number[3];
                snprintf(number, sizeof(number), "%d", (k - 7 * 120) / 60 + 1);
                strcat(coach, number);
                system->trains[i].seats[j] = (Seat){seat_number, 0, "AC Chair Car", "Window", 400.0};
                strcpy(system->trains[i].seats[j].coach, coach);
                k++;
            }
            for (int j = 7 * 120 + 5 * 60; j < total_seats; j++, seat_number++) {
                char coach[3] = "E";
                char number[3];
                snprintf(number, sizeof(number), "%d", (k - 7 * 120 - 5 * 60) / 45 + 1);
                strcat(coach, number);
                system->trains[i].seats[j] = (Seat){seat_number, 0, "Executive Chair Car", "Window", 600.0};
                strcpy(system->trains[i].seats[j].coach, coach);
                k++;
            }
        }
    }
}

const char* get_seat_type(int seat_number) {
    switch ((seat_number - 1) % 6) {
        case 0: return "Window";
        case 1: return "Middle";
        case 2: return "Aisle";
        case 3: return "Aisle";
        case 4: return "Middle";
        case 5: return "Window";
    }
    return "Invalid Seat Number";
}

void allocate_seat_or_berth(Train *train, const char *preference, const char *class_type, int *allocated_seat_number, char *allocated_seat_type, char *allocated_coach, double *fare) {
    for (int i = 0; i < train->total_seats; i++) {
        if (!train->seats[i].is_booked && strcmp(get_seat_type(train->seats[i].seat_number), preference) == 0 && strstr(train->seats[i].preference, class_type)) {
            train->seats[i].is_booked = 1;
            *allocated_seat_number = train->seats[i].seat_number;
            strcpy(allocated_seat_type, get_seat_type(train->seats[i].seat_number));
            strcpy(allocated_coach, train->seats[i].coach);
            *fare = train->seats[i].fare;
            train->available_seats--;
            return;
        }
    }
    // Allocate first available seat if preferred is not available
    for (int i = 0; i < train->total_seats; i++) {
        if (!train->seats[i].is_booked && strstr(train->seats[i].preference, class_type)) {
            train->seats[i].is_booked = 1;
            *allocated_seat_number = train->seats[i].seat_number;
            strcpy(allocated_seat_type, get_seat_type(train->seats[i].seat_number));
            strcpy(allocated_coach, train->seats[i].coach);
            *fare = train->seats[i].fare;
            train->available_seats--;
            return;
        }
    }
}

void free_seat(Train *train, int seat_number) {
    for (int i = 0; i < train->total_seats; i++) {
        if (train->seats[i].seat_number == seat_number) {
            train->seats[i].is_booked = 0;
            train->available_seats++;
            return;
        }
    }
}

void display_available_pnrs() {
    printf("%s",green);
    printf("-->Available PNRs:\n");

    for (int i = 0; i < booking_count; i++) {
        pnrcheck=1;
        printf("-->%d\n", bookings[i].pnr);
    }
    printf("%s",reset);
}

void add_to_waiting_list(Train *train, const char *date, const char *class_type, const char *username) {
    Ticket wait_ticket = {
        .pnr = waiting_count + 1,
        .seat_number = -1,
        .date = ""
    };

    strcpy(wait_ticket.train_name, train->name);
    strcpy(wait_ticket.from, train->from);
    strcpy(wait_ticket.to, train->to);
    strcpy(wait_ticket.date, date);
    strcpy(wait_ticket.coach, class_type);
    strcpy(wait_ticket.username, username);

    waiting_list[waiting_count++] = wait_ticket;
    save_waiting_list();
    printf("%s",yellow);
    printf("--->All seats are booked. You've been added to the waiting list<---\n\n");
    printf("---Your waiting list number is: %d---\n", wait_ticket.pnr);
    printf("%s",reset);
}

void allocate_waiting_list_seat(RailwaySystem *system, Train *train) {
    for (int i = 0; i < waiting_count; i++) {
        if (strcmp(waiting_list[i].train_name, train->name) == 0 && strcmp(waiting_list[i].from, train->from) == 0 && strcmp(waiting_list[i].to, train->to) == 0) {
            int allocated_seat_number = -1;
            char allocated_seat_type[20];
            char allocated_coach[3];
            double fare;

            allocate_seat_or_berth(train, "any", waiting_list[i].coach, &allocated_seat_number, allocated_seat_type, allocated_coach, &fare);

            if (allocated_seat_number != -1) {
                Ticket new_booking = {
                    .pnr = booking_count + 1,
                    .seat_number = allocated_seat_number,
                    .date = ""
                };
                strcpy(new_booking.train_name, waiting_list[i].train_name);
                strcpy(new_booking.from, waiting_list[i].from);
                strcpy(new_booking.to, waiting_list[i].to);
                strcpy(new_booking.date, waiting_list[i].date);
                strcpy(new_booking.coach, allocated_coach);
                strcpy(new_booking.username, waiting_list[i].username);
                if (strcmp(train->type, "Day") == 0) {
                    snprintf(new_booking.seat_type, sizeof(new_booking.seat_type), "%s (Seater)", allocated_seat_type);
                } else {
                    snprintf(new_booking.seat_type, sizeof(new_booking.seat_type), "%s (Berth)", allocated_seat_type);
                }
                bookings[booking_count++] = new_booking;

                for (int j = i; j < waiting_count - 1; j++) {
                    waiting_list[j] = waiting_list[j + 1];
                }
                waiting_count--;
                save_bookings();
                save_waiting_list();
                save_seat_status(system);
                printf("%s",green);
                printf("-->Ticket confirmed from waiting list. PNR: %d\n", new_booking.pnr);
                printf("%s",reset);
                return;
            }
        }
    }
}

void displaySeatingArrangement() {
    printf("%s",red);
        printf("........................................\n");
    printf(". Second Seating: 3*3                 .\n");
    printf(". Push Back Seating Arrangement       .\n");
    printf(". To Seat: 108.                       .\n");
    printf("........................................\n\n");
    printf("%s",reset);
    printf("%s",yellow);
    printf("  |  W | M | A |       | A | M | W |\n");
    printf("--+----+---+---+-------+---+---+----+--\n");
    printf("  | 108|107|106|       |105|104|103|\n");
    printf("--+----+---+---+-------+---+---+----+--\n");
    printf("  | 102|101|100|       | 99| 98| 97|\n");
    printf("--+----+---+---+-------+---+---+----+--\n");
    printf("  |  96| 95| 94|       | 93| 92| 91|\n");
    printf("--+----+---+---+-------+---+---+----+--\n");
    printf("  |  90| 89| 88|       | 87| 86| 85|\n");
    printf("--+----+---+---+-------+---+---+----+--\n");
    printf("  |  84| 83| 82|       | 81| 80| 79|\n");
    printf("--+----+---+---+-------+---+---+----+--\n");
    printf("  |  78| 77| 76|       | 75| 74| 73|\n");
    printf("--+----+---+---+-------+---+---+----+--\n");
    printf("  |  72| 71| 70|       | 69| 68| 67|\n");
    printf("--+----+---+---+-------+---+---+----+--\n");
    printf("  |  66| 65| 64|       | 63| 62| 61|\n");
    printf("--+----+---+---+-------+---+---+----+--\n");
    printf("  |  60| 59| 58|       | 57| 56| 55|\n");
    printf("--+----+---+---+-------+---+---+----+--\n");
    printf("  |  54| 53| 52|       | 51| 50| 49|\n");
    printf("--+----+---+---+-------+---+---+----+--\n");
    printf("  |  48| 47| 46|       | 45| 44| 43|\n");
    printf("--+----+---+---+-------+---+---+----+--\n");
    printf("  |  42| 41| 40|       | 39| 38| 37|\n");
    printf("--+----+---+---+-------+---+---+----+--\n");
    printf("  |  36| 35| 34|       | 33| 32| 31|\n");
    printf("--+----+---+---+-------+---+---+----+--\n");
    printf("  |  30| 29| 28|       | 27| 26| 25|\n");
    printf("--+----+---+---+-------+---+---+----+--\n");
    printf("  |  24| 23| 22|       | 21| 20| 19|\n");
    printf("--+----+---+---+-------+---+---+----+--\n");
    printf("  |  18| 17| 16|       | 15| 14| 13|\n");
    printf("--+----+---+---+-------+---+---+----+--\n");
    printf("  |  12| 11| 10|       |  9|  8|  7|\n");
    printf("--+----+---+---+-------+---+---+----+--\n");
    printf("  |   6|  5|  4|       |  3|  2|  1|\n");
    printf("--+----+---+---+-------+---+---+----+--\n");

    printf("\nLegend:\n");
    printf("W - Window Seat\n");
    printf("A - Aisle Seat\n");
    printf("M - Middle Seat\n");

    printf("%s",reset);
}

void book_ticket(RailwaySystem *system, int user_index) {
    char from[MAX_NAME_LENGTH];
    char to[MAX_NAME_LENGTH];
    char date[11];
    char passenger_details[MAX_PASSENGERS][3][MAX_NAME_LENGTH]; // To store name, age, gender for each passenger
    char preference[20];
    char class_type[20];
    int num_passengers;
    clear_screen();
    printf("%s",yellow);
    printf("\n*****************\n");
    printf("* Book a Ticket *\n");
    printf("*****************\n\n");

    printf("Enter the travel date (YYYY-MM-DD): ");
    scanf("%s", date);
    printf("Enter the origin station: ");
    scanf("%s", from);
    printf("Enter the destination station: ");
    scanf("%s", to);
    printf("%s",reset);
    printf("%s",cyan);
    displaySeatingArrangement();
    printf("\nAvailable trains from %s to %s on %s:\n", from, to, date);
    printf("-------------------------------------------------\n");
    printf("%s",reset);
    int found = 0, direct = 0;

    // Search for direct trains
    for (int i = 0; i < MAX_TRAINS; i++) {
        if (strcmp(system->trains[i].from, from) == 0 && strcmp(system->trains[i].to, to) == 0) {
            printf("%d. %s (Departure: %s, Arrival: %s, %s train)\n", 
                    i + 1, system->trains[i].name, system->trains[i].departure_time, 
                    system->trains[i].arrival_time, system->trains[i].type);
                     //printf("----------------------------------------------------------\n");
            found = 1;
            direct = 1;
        }
    }

    // If no direct trains, search for connecting trains
    if (direct == 0 && found != 1) {printf("%s",red);
        printf("\n-->No direct trains available!!\n\n-->The following connecting trains are available:\n");
        printf("-------------------------------------------------\n");printf("%s",reset);
        printf("%s",cyan);
        for (int m = 0; m < 10; ++m) {
            if (strcmp(system->trains[m].from, from) == 0) {
                
                printf("%d. %s (Departure from %s at: %s, Arrival to %s at: %s, %s train)\n", 
                        system->trains[m].train_number, system->trains[m].name,system->trains[m].from, 
                        system->trains[m].departure_time,system->trains[m].to, system->trains[m].arrival_time, 
                        system->trains[m].type);
                break;
            }
        }
        for (int k = 0; k < 10; ++k) {
            if (strcmp(system->trains[k].to, to) == 0) {
                   printf("%d. %s (Departure from %s at: %s, Arrival to %s at: %s, %s train)\n", 
                        system->trains[k].train_number, system->trains[k].name,system->trains[k].from, 
                        system->trains[k].departure_time,system->trains[k].to, system->trains[k].arrival_time, 
                        system->trains[k].type);
                
                break;
            }
        }
        found = 1;printf("%s",reset);
    }

    if (!found) {
        printf("\nNo trains found for the specified route and date.\n");
    printf("-------------------------------------------------\n");
        return;
    }

    int train_choice;
    printf("-------------------------------------------------\n");
    printf("Choose a train (1-%d): ", MAX_TRAINS);
    scanf("%d", &train_choice);

    if (train_choice < 1 || train_choice > MAX_TRAINS) {
        printf("Invalid choice.\n");
        return;
    }

    Train *train = &system->trains[train_choice - 1];

    // Display available seats in each class
    int sleeper_available = 0, ac3tier_available = 0, ac2tier_available = 0, acfirst_available = 0;
    int general_available = 0, acchaircar_available = 0, executivechaircar_available = 0;

    for (int i = 0; i < train->total_seats; i++) {
        if (!train->seats[i].is_booked) {
            if (strstr(train->seats[i].preference, "Sleeper")) sleeper_available++;
            else if (strstr(train->seats[i].preference, "AC 3 Tier")) ac3tier_available++;
            else if (strstr(train->seats[i].preference, "AC 2 Tier")) ac2tier_available++;
            else if (strstr(train->seats[i].preference, "AC First")) acfirst_available++;
            else if (strstr(train->seats[i].preference, "General")) general_available++;
            else if (strstr(train->seats[i].preference, "AC Chair Car")) acchaircar_available++;
            else if (strstr(train->seats[i].preference, "Executive Chair Car")) executivechaircar_available++;
        }
    }
    printf("%s",green);
    printf("\nAvailable Classes:\n");
    printf("-------------------------------------------------\n");
    if (strcmp(train->type, "Night") == 0) {
        printf("1. Sleeper: %d seats\n", sleeper_available);
        printf("2. AC 3 Tier: %d seats\n", ac3tier_available);
        printf("3. AC 2 Tier: %d seats\n", ac2tier_available);
        printf("4. AC First: %d seats\n", acfirst_available);
    } else if (strcmp(train->type, "Day") == 0) {
        printf("1. General: %d seats\n", general_available);
        printf("2. AC Chair Car: %d seats\n", acchaircar_available);
        printf("3. Executive Chair Car: %d seats\n", executivechaircar_available);
    }

    int class_choice;
    printf("------------------------------------------------------\n\n");
        printf("%s",reset);
    printf("Choose a class (1-4 for Night train, 1-3 for Day train): ");
    scanf("%d", &class_choice);
    printf("------------------------------------------------------\n\n");

    if (strcmp(train->type, "Night") == 0) {
        switch (class_choice) {
            case 1:
                strcpy(class_type, "Sleeper");
                break;
            case 2:
                strcpy(class_type, "AC 3 Tier");
                break;
            case 3:
                strcpy(class_type, "AC 2 Tier");
                break;
            case 4:
                strcpy(class_type, "AC First");
                break;
            default:
                printf("Invalid choice.\n");
                return;
        }
    } else if (strcmp(train->type, "Day") == 0) {
        switch (class_choice) {
            case 1:
                strcpy(class_type, "General");
                break;
            case 2:
                strcpy(class_type, "AC Chair Car");
                break;
            case 3:
                strcpy(class_type, "Executive Chair Car");
                break;
            default:
                printf("Invalid choice.\n");
                return;
        }
    }

    printf("\nEnter the number of passengers: ");
    scanf("%d", &num_passengers);

    double total_fare = 0.0;
    int seats_available = train->available_seats;
   printf("\033[2J\033[1;1H");

    // Print Passenger Details Header
    printf("\n\n\033[1;33m");
    printf("************************\n");
    printf("*   Passenger Details  *\n");
    printf("************************\033[0m\n");

    for (int i = 0; i < num_passengers; i++) {
        printf("%s",green);
        printf("\nEnter passenger %d name: ", i + 1);
        scanf("%s", passenger_details[i][0]);
        strcpy(bookings[i].passenger_name, passenger_details[i][0]);
        printf("Enter passenger %d age: ", i + 1);
        scanf("%s", passenger_details[i][1]);
        printf("Enter passenger %d gender: ", i + 1);
        scanf("%s", passenger_details[i][2]);

        printf("Enter seat preference (Aisle/Window for Day, Lower/Upper for Night): ");
        scanf("%s", preference);
    printf("%s",reset);
        int allocated_seat_number = -1;
        char allocated_seat_type[20];
        char allocated_coach[3];
        double fare;

        if (seats_available > 0) {
            allocate_seat_or_berth(train, preference, class_type, &allocated_seat_number, allocated_seat_type, allocated_coach, &fare);
            seats_available--;
        }

        if (allocated_seat_number != -1) {
            printf("\n");
        printf("%s",cyan);
       char border[] = "\033[1;34m******************\033[0m"; // Blue color
    char ticket_title[] = "\033[1;34m*Ticket          *\033[0m"; // Blue color
    char pnr_label[50], seat_label[50], type_label[50], coach_label[50];

    // Format the labels with the ticket details
    snprintf(pnr_label, sizeof(pnr_label), "\033[1;34m* PNR: %d                          \033[0m", booking_count+1); // Blue color
    snprintf(seat_label, sizeof(seat_label), "\033[1;34m* Allocated Seat: %d                \033[0m", allocated_seat_number); // Blue color
    snprintf(type_label, sizeof(type_label), "\033[1;34m* Type: %s                      \033[0m", allocated_seat_type); // Blue color
    snprintf(coach_label, sizeof(coach_label), "\033[1;34m* Coach: %s                        \033[0m", allocated_coach); // Blue color

    // Print the ticket in the desired format
    printf("%s\n", border);
    printf("%s\n", ticket_title);
    printf("%s\n", border);
    printf("%s\n", pnr_label);
    printf("%s\n", seat_label);
    printf("%s\n", type_label);
    printf("%s\n", coach_label);
    printf("%s\n", border);
            // Save booking temporarily
            bookings[booking_count] = (Ticket){
                .pnr = booking_count + 1,
                .seat_number = allocated_seat_number,
                .date = "",
                .username = "" // Initialize username field
            };
            strcpy(bookings[booking_count].train_name, train->name);
            strcpy(bookings[booking_count].from, train->from);
            strcpy(bookings[booking_count].to, train->to);
            strcpy(bookings[booking_count].date, date);
            strcpy(bookings[booking_count].coach, allocated_coach);
            strcpy(bookings[booking_count].username, users[user_index - 1].username); // Add username to ticket
            if (strcmp(train->type, "Day") == 0) {
                snprintf(bookings[booking_count].seat_type, sizeof(bookings[booking_count].seat_type), "%s (Seater)", allocated_seat_type);
            } else {
                snprintf(bookings[booking_count].seat_type, sizeof(bookings[booking_count].seat_type), "%s (Berth)", allocated_seat_type);
            }
            booking_count++;
            total_fare += fare;
        } else {
            add_to_waiting_list(train, date, class_type, users[user_index - 1].username);
        }
    }

    if (total_fare > 0 && !process_payment(total_fare, users[user_index - 1].payment_password)) {
        printf("\nBooking failed due to payment issue. Cancelling seats...\n");
        // Free allocated seats in case of payment failure
        for (int i = booking_count - num_passengers; i < booking_count; i++) {
            free_seat(train, bookings[i].seat_number);
        }
        booking_count -= num_passengers; // Rollback the booking count
        return;
    }

    save_bookings();
    save_seat_status(system);
}


void cancel_ticket(RailwaySystem *system, int user_index) {
    printf("%s",cyan);
    printf("Previously booked tickets:\n\n");
    display_available_pnrs();

    int pnr;
    printf("Enter the PNR of the ticket to be cancelled:\n ");
    scanf("%d", &pnr);

    char cancellation_date[11];
    time_t now = time(NULL);
    strftime(cancellation_date, sizeof(cancellation_date), "%Y-%m-%d", localtime(&now));

    for (int i = 0; i < booking_count; i++) {
        if (bookings[i].pnr == pnr) {
            // Find the train
            for (int j = 0; j < MAX_TRAINS; j++) {
                if (strcmp(system->trains[j].name, bookings[i].train_name) == 0) {
                    Train *train = &system->trains[j];

                    // Find the seat
                    for (int k = 0; k < train->total_seats; k++) {
                        if (train->seats[k].seat_number == bookings[i].seat_number) {
                            double fare = 0.0;
                            for (int l = 0; l < train->total_seats; l++) {
                                if (train->seats[l].seat_number == bookings[i].seat_number) {
                                    fare = train->seats[l].fare;
                                    break;
                                }
                            }
                            double refund_amount = calculate_refund_amount(bookings[i].date, cancellation_date, fare);

                            if (refund_amount > 0) {
                                printf("-->Refund amount: Rs.%.2f\n", refund_amount);
                            } else {
                                printf("-->No refund will be issued for this cancellation.\n");
                            }

                            train->seats[k].is_booked = 0;
                            train->available_seats++;

                            // Remove booking
                            bookings[i] = bookings[booking_count - 1];
                            booking_count--;

                            save_bookings();
                            save_seat_status(system);

                            printf("-->Ticket cancelled successfully.\n");

                            // Update waiting list
                            allocate_waiting_list_seat(system, train);
                            
                            return;
                        }
                    }
                }
            }
        }
    }

    printf("PNR not found.\n");
}

double calculate_probability_of_confirmation(int waitlist_position, int available_seats) {
    if (available_seats == 0) {
        return 0.0;
    }
    return (double)available_seats / (available_seats + waitlist_position);
}

void view_waitlist_status() {
    printf("%s",yellow);
    if (waiting_count == 0) {

        printf("-->No tickets in the waiting list!!.<--\n");
        return;
    }

    for (int i = 0; i < waiting_count; i++) {
    
    printf("******************************************\n");
    printf("*             INDIAN RAILWAYS             *\n");
    printf("******************************************\n\n");
    printf("Waitlist Position: %d\n", i + 1);
    printf("PNR: %d\n", waiting_list[i].pnr);
    printf("Train: %s\n", waiting_list[i].train_name);
    printf("From: %s\n", waiting_list[i].from);
    printf("To: %s\n", waiting_list[i].to);
    printf("Date: %s\n", waiting_list[i].date);
    printf("Seat: %d\n", waiting_list[i].seat_number);
    printf("Coach: %s\n", waiting_list[i].coach);
    printf("Seat Type: %s\n", waiting_list[i].seat_type);
    double probability = calculate_probability_of_confirmation(i + 1, 10);
    printf("Probability of Confirmation: %.2f%%\n", probability * 100);
    printf("******************************************\n\n");
   
    }
     printf("%s",reset);
}

int main() {
    RailwaySystem system;
    initialize_system(&system);
    load_users();
    load_bookings();
    load_seat_status(&system);
    load_waiting_list();
    clear_screen();
    int choice;
    while (1) {
        printf("%s",purple);
    printf("*******************************************\n");
    printf("* WELCOME TO RAILWAY RESERVATION SYSTEM!..*\n");
    printf("*******************************************\n\n");
    printf("* 1. New user signup.*\n");
    printf("* 2. Existing user login.*\n");
    printf("* 3. Forgot password.*\n");
    printf("* 4. Exit main menu.*\n");
    
   
    
    printf("\n****************\n");
     printf("Enter your choice from above options: ");
     printf("%s",reset);
     scanf("%d", &choice);
     
     clear_screen();
   
        

        if (choice == 1) {
            signup();
        } else if (choice == 2) {
            int user_index = login();
            if (user_index) {
                while (1) {printf("%s",purple);
    printf("******************************************\n\n");
    printf("*            USER MENU                   *\n\n");
    printf("******************************************\n\n");
    printf("* 1. Book Ticket                         *\n");
    printf("* 2. Cancel Ticket                       *\n");
    printf("* 3. View Ticket                         *\n");
    printf("* 4. View Waitlist Status                *\n");
    printf("* 5. Logout                              *\n");
    printf("******************************************\n");
    printf("Enter your choice: ");printf("%s",reset);
                    scanf("%d", &choice);

                    if (choice == 1) {
                        
                        book_ticket(&system, user_index);
                    } else if (choice == 2) {
                        cancel_ticket(&system, user_index);
                    } else if (choice == 3) {
                        view_ticket();
                    } else if (choice == 4) {
                        view_waitlist_status();
                    } else if (choice == 5) {
                        break;
                    } else {
                        printf("Invalid choice.\n");
                    }
                }
            }
        } else if (choice == 3) {
            retrieve_password();
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}