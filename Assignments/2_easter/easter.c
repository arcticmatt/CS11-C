#include <stdio.h>
#include <string.h>
/* Program that, given a year (or years)
 * calculates the month and day of easter
 * and prints out the information
 * in the format of "year - month day"
 */

/* Function prototype */
int calculate_Easter_date(int year);

int main(void)
{
  /* Initialize variables */
  int day;
  int year;
  char month[100];
  int break_int;

  /* Loops through every line of the input file, and
   * breaks out when the EOF is reached
   */
  while (1)
  {
    /* When there is no more input,
     * break_char will equal the integer constant
     * EOF
     */
    break_int = scanf("%d", &year);
    day = calculate_Easter_date(year);
    /* If EOF is reached, break */
    if (break_int == EOF)
    {
      break;
    }
    /* Else if day is not 0, see if the
     * month is March or April.
     */
    else if (day != 0)
    {
      if (day < 0)
      {
        strcpy(month, "March");
        /* Change the day to a positive integer */
        day = -day;
      }
      else if (day > 0)
      {
        strcpy(month, "April");
      }
      /* Print the year, month, and day */
      printf("%d - %s %d\n", year, month, day);
    }
    /* Else if day is 0 (meaning the year was out of range)
     * then print an error message
     */
    else
    {
      fprintf(stderr, "Error - year %d out of range\n", year);
    }
  }
  return 0;
}

/* Does: This function calculates the day and month
 * upon which easter falls on a given year.
 * Arguments: It takes the year as an argument
 * Returns: An integer that represents the day of easter.
 * Integer is negative if the day is in March
 * and integer is positive if the day is in April.
 * Integer is 0 if the year is out of range.
 */
int calculate_Easter_date(int year)
{
  /* If the year is out of range, return 0 */
  if (year > 39999 || year <1582)
  {
    return 0;
  }
  /* Else, calculate the date */
  else
  {
    /* Initialize all variables */
    int golden_year;
    int century;
    int skipped_leap_years;
    int moon_orbit_correction;
    int sunday_finder;
    int epact;
    int day;

    /* Number of years into the Metonic cycle
     * the given year is
     */
    golden_year = (year % 19) + 1;
    /* Century of the year */
    century = (year / 100) + 1;
    /* Number of skipped leap years. Leap years
     * are skipped if the current year is a multiple of 100,
     * unless the current year is also a multiple of 400
     */
    skipped_leap_years = (3 * century / 4) - 12;
    /* A correction factor that accounts for the fact that the
     * moon orbits the earth slightly less than 235 times
     * in 19 years
     */
    moon_orbit_correction = ((8 * century + 5) / 25) - 5;
    /* Number that is used to find a Sunday in March */
    sunday_finder = (5 * year / 4) - skipped_leap_years - 10;
    /* Number of days since a new moon */
    epact = (11 * golden_year + 20 + moon_orbit_correction -
        skipped_leap_years) % 30;
    /* Increment epact under certain conditions */
    if ((epact == 25 && golden_year > 11) || (epact == 24))
    {
      epact++;
    }
    /* March (day) is a "calendar full moon" */
    day = 44 - epact;
    if (day < 21)
    {
      day += 30;
    }
    /* The day is now a Sunday after the full moon */
    day = day + 7 - ((sunday_finder + day) % 7);
    /* If the day is greater than 31, the date is
     * April (day - 31)
     */
    if (day > 31)
    {
      return day - 31;
    }
    /* Else, the date is March (day). Return a negative
     * value to indicate the month
     */
    else
    {
      return -day;
    }
  }

}
