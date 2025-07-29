<h1 align="center">Railway Reservation System</h1>

<h2>Overview</h2>
<p>This is a comprehensive Railway Reservation System implemented in C, designed to simulate the booking and management of train tickets. The system includes features for user authentication, ticket booking, cancellation, viewing ticket details, and managing waiting lists.</p>

<h2>Features</h2>

<h3>User Management</h3>
<ul>
  <li><strong>Signup</strong>: New users can create an account with a username, password, payment password, and security questions.</li>
  <li><strong>Login</strong>: Existing users can log in to access the booking system.</li>
  <li><strong>Password Recovery</strong>: Users can retrieve forgotten passwords by answering security questions.</li>
</ul>

<h3>Ticket Booking</h3>
<ul>
  <li><strong>Train Selection</strong>: Users can view available trains between specified stations and dates.</li>
  <li><strong>Seat Selection</strong>: Users can choose from different classes (Sleeper, AC 3 Tier, AC 2 Tier, etc.) and seat preferences (Window, Aisle, etc.).</li>
  <li><strong>Payment Processing</strong>: Secure payment processing with payment password verification.</li>
  <li><strong>Waiting List</strong>: If seats are unavailable, users can be added to a waiting list with a probability of confirmation.</li>
</ul>

<h3>Ticket Management</h3>
<ul>
  <li><strong>View Ticket</strong>: Users can view their booked ticket details using a PNR number.</li>
  <li><strong>Cancel Ticket</strong>: Users can cancel tickets, with refunds calculated based on cancellation time.</li>
  <li><strong>Seat Status</strong>: Real-time seat availability and booking status.</li>
</ul>

<h3>Additional Features</h3>
<ul>
  <li><strong>Seating Arrangement</strong>: Visual representation of seating arrangements.</li>
  <li><strong>Waitlist Status</strong>: View current waitlist position and probability of confirmation.</li>
  <li><strong>Data Persistence</strong>: All user data, bookings, and seat statuses are saved to files for persistence between sessions.</li>
</ul>

<h2>Code Structure</h2>

<h3>Data Structures</h3>
<ul>
  <li><strong>User</strong>: Stores user credentials, payment details, and security questions.</li>
  <li><strong>Ticket</strong>: Contains ticket details like PNR, passenger info, train details, and seat information.</li>
  <li><strong>Train</strong>: Defines train properties including name, schedule, seat configuration, and availability.</li>
  <li><strong>Seat</strong>: Represents individual seat properties like number, booking status, and fare.</li>
</ul>

<h3>Key Functions</h3>
<ul>
  <li><strong>User Authentication</strong>: <code>signup()</code>, <code>login()</code>, <code>retrieve_password()</code></li>
  <li><strong>Ticket Operations</strong>: <code>book_ticket()</code>, <code>cancel_ticket()</code>, <code>view_ticket()</code></li>
  <li><strong>Seat Management</strong>: <code>allocate_seat_or_berth()</code>, <code>free_seat()</code>, <code>displaySeatingArrangement()</code></li>
  <li><strong>File Handling</strong>: <code>load_users()</code>, <code>save_bookings()</code>, <code>load_seat_status()</code>, etc.</li>
</ul>

<h2>Usage</h2>

<h3>Compilation</h3>
<p>Compile the code using a C compiler (e.g., gcc).</p>
<pre><code>gcc railway_system.c -o railway_system</code></pre>

<h3>Execution</h3>
<p>Run the compiled program.</p>
<pre><code>./railway_system</code></pre>

<h3>Navigation</h3>
<ol>
  <li>Sign up or log in as an existing user.</li>
  <li>Use the menu to book, cancel, or view tickets.</li>
  <li>View waitlist status if applicable.</li>
</ol>

<h2>Data Files</h2>
<p>The system uses several data files to persist information:</p>
<ul>
  <li><code>users.dat</code>: Stores user account information.</li>
  <li><code>bookings.dat</code>: Contains all booked tickets.</li>
  <li><code>waiting_list.dat</code>: Manages waiting list entries.</li>
  <li><code>seat_status.dat</code>: Tracks seat availability for each train.</li>
</ul>

<h2>Dependencies</h2>
<p>Standard C libraries: <code>stdio.h</code>, <code>stdlib.h</code>, <code>string.h</code>, <code>ctype.h</code>, <code>time.h</code></p>

<h2>Notes</h2>
<ul>
  <li>The system includes ANSI color codes for better visual presentation.</li>
  <li>All monetary values are in Indian Rupees (INR).</li>
  <li>The system currently supports a fixed set of trains and stations (primarily in Tamil Nadu, India).</li>
</ul>

