#include <stdlib.h>             // malloc
#include <string.h>             // strncmp
#include <stdbool.h>            // bool, true, false
#include <stddef.h>             // size_t

#include "ssct.h"
#include "schedr_status_codes.h" // Status, error codes
#include "schedr_job.h"       // Job, schedr_job_set_*()

// Test prototypes
static void should_set_all_job_members_when_setters_are_called();
static void job_setters_should_set_all_job_members_when_job_is_dynamically_allocated();

static void init_should_return_null_argument_error_when_job_argument_is_null();
static void init_should_set_members_to_default_values();

static void set_name_should_set_name_member_when_argument_consists_of_printable_ascii_symbols();
static void set_name_should_return_invalid_argument_error_when_name_argument_contains_non_printable_ascii_symbols();
static void set_name_should_return_null_argument_error_when_job_argument_is_null();
static void set_name_should_return_null_argument_error_when_name_argument_is_null();
static void set_name_should_return_invalid_argument_error_when_name_len_argument_is_equal_to_zero();
static void set_name_should_set_entire_name_member_when_name_argument_length_is_equal_to_max_allowed();
static void set_name_should_return_buffer_overflow_error_when_name_argument_length_is_longer_than_max_allowed();
static void set_name_should_return_invalid_argument_error_when_name_argument_is_an_empty_string();

static void set_command_should_set_command_member_when_command_argument_is_a_valid_command();
static void set_command_should_return_null_argument_error_when_job_argument_is_null();
static void set_command_should_return_null_argument_error_when_command_argument_is_null();
static void set_command_should_return_invalid_argument_error_when_cmd_len_argument_is_equal_to_zero();
static void set_command_should_return_invalid_argument_error_when_command_argument_is_an_empty_string();
static void set_command_should_set_entire_command_member_when_command_argument_length_is_equal_to_max_allowed();
static void set_command_should_return_buffer_overflow_error_when_command_argument_length_is_longer_than_max_allowed();

static void set_interval_should_return_null_argument_error_when_job_argument_is_null();
static void set_interval_should_return_invalid_argument_error_when_interval_argument_is_negative();

static void set_state_should_return_null_argument_error_when_job_argument_is_null();
static void set_state_should_return_invalid_argument_error_when_state_argument_is_negative();
static void set_state_should_return_invalid_argument_error_when_state_argument_is_out_of_range();

int main(void)
{
    ssct_run(should_set_all_job_members_when_setters_are_called);
    ssct_run(job_setters_should_set_all_job_members_when_job_is_dynamically_allocated);

    ssct_run(init_should_return_null_argument_error_when_job_argument_is_null);
    ssct_run(init_should_set_members_to_default_values);

    ssct_run(set_name_should_set_name_member_when_argument_consists_of_printable_ascii_symbols);
    ssct_run(set_name_should_return_invalid_argument_error_when_name_argument_contains_non_printable_ascii_symbols);
    ssct_run(set_name_should_return_invalid_argument_error_when_name_len_argument_is_equal_to_zero);
    ssct_run(set_name_should_return_null_argument_error_when_job_argument_is_null);
    ssct_run(set_name_should_return_null_argument_error_when_name_argument_is_null);
    ssct_run(set_name_should_set_entire_name_member_when_name_argument_length_is_equal_to_max_allowed);
    ssct_run(set_name_should_return_buffer_overflow_error_when_name_argument_length_is_longer_than_max_allowed);
    ssct_run(set_name_should_return_invalid_argument_error_when_name_argument_is_an_empty_string);

    ssct_run(set_command_should_set_command_member_when_command_argument_is_a_valid_command);
    ssct_run(set_command_should_return_null_argument_error_when_job_argument_is_null);
    ssct_run(set_command_should_return_null_argument_error_when_command_argument_is_null);
    ssct_run(set_command_should_return_invalid_argument_error_when_command_argument_is_an_empty_string);
    ssct_run(set_command_should_return_invalid_argument_error_when_cmd_len_argument_is_equal_to_zero);
    ssct_run(set_command_should_set_entire_command_member_when_command_argument_length_is_equal_to_max_allowed);
    ssct_run(set_command_should_return_buffer_overflow_error_when_command_argument_length_is_longer_than_max_allowed);

    ssct_run(set_interval_should_return_null_argument_error_when_job_argument_is_null);
    ssct_run(set_interval_should_return_invalid_argument_error_when_interval_argument_is_negative);

    ssct_run(set_state_should_return_null_argument_error_when_job_argument_is_null);
    ssct_run(set_state_should_return_invalid_argument_error_when_state_argument_is_negative);
    ssct_run(set_state_should_return_invalid_argument_error_when_state_argument_is_out_of_range);

    ssct_print_summary();

    return EXIT_SUCCESS;
}

static void init_should_return_null_argument_error_when_job_argument_is_null()
{
    Status status = schedr_job_init(NULL);

    ssct_assert_equals(status, SCHEDR_ERROR_NULL_ARGUMENT);
}

static void init_should_set_members_to_default_values()
{
    Job job;

    Status status = schedr_job_init(&job);

    ssct_assert_empty(job.name);
    ssct_assert_empty(job.command);
    ssct_assert_zero(job.interval_seconds);
    ssct_assert_equals(job.state, Stopped);
    ssct_assert_equals(status, SCHEDR_SUCCESS);
}

static void should_set_all_job_members_when_setters_are_called()
{
    static const char JOB_NAME[] = "test job";
    static const int JOB_NAME_LEN = sizeof (JOB_NAME) - 1;
    static const char JOB_CMD[] = "echo testing";
    static const int JOB_CMD_LEN = sizeof (JOB_CMD) - 1;
    static const int JOB_INTERVAL = 10;
    static const JobState JOB_STATE = Running;

    Job job;
    
    Status name_status = schedr_job_set_name(&job, JOB_NAME, JOB_NAME_LEN);
    Status cmd_status = schedr_job_set_command(&job, JOB_CMD, JOB_CMD_LEN);
    Status interval_status = schedr_job_set_interval(&job, JOB_INTERVAL);
    Status state_status = schedr_job_set_state(&job, JOB_STATE);

    ssct_assert_equals(name_status, SCHEDR_SUCCESS);
    ssct_assert_equals(cmd_status, SCHEDR_SUCCESS);
    ssct_assert_equals(interval_status, SCHEDR_SUCCESS);
    ssct_assert_equals(state_status, SCHEDR_SUCCESS);

    ssct_assert_equals(job.name, strlen(job.name), JOB_NAME, JOB_NAME_LEN);
    ssct_assert_equals(job.command, strlen(job.command), JOB_CMD, JOB_CMD_LEN);
    ssct_assert_equals(job.interval_seconds, JOB_INTERVAL);
    ssct_assert_equals(job.state, JOB_STATE);
}

static void job_setters_should_set_all_job_members_when_job_is_dynamically_allocated()
{
    static const char JOB_NAME[] = "dyn job";
    static const int JOB_NAME_LEN = sizeof (JOB_NAME) - 1;
    static const char JOB_CMD[] = "echo dynamic";
    static const int JOB_CMD_LEN = sizeof (JOB_CMD) - 1;
    static const int JOB_INTERVAL = 5;
    static const JobState JOB_STATE = Stopped;

    Job *job_p = (Job *)malloc(sizeof (Job));

    Status name_status = schedr_job_set_name(job_p, JOB_NAME, JOB_NAME_LEN);
    Status cmd_status = schedr_job_set_command(job_p, JOB_CMD, JOB_CMD_LEN);
    Status interval_status = schedr_job_set_interval(job_p, JOB_INTERVAL);
    Status state_status = schedr_job_set_state(job_p, JOB_STATE);

    ssct_assert_equals(name_status, SCHEDR_SUCCESS);
    ssct_assert_equals(cmd_status, SCHEDR_SUCCESS);
    ssct_assert_equals(interval_status, SCHEDR_SUCCESS);
    ssct_assert_equals(state_status, SCHEDR_SUCCESS);

    ssct_assert_equals(job_p->name, strlen(job_p->name), JOB_NAME, JOB_NAME_LEN);
    ssct_assert_equals(job_p->command, strlen(job_p->command), JOB_CMD, JOB_CMD_LEN);
    ssct_assert_equals(job_p->interval_seconds, JOB_INTERVAL);
    ssct_assert_equals(job_p->state, JOB_STATE);

    free(job_p);
}

static void set_name_should_set_name_member_when_argument_consists_of_printable_ascii_symbols()
{
    static const char PRINTABLE_ASCII_CHARS[] = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    static const int PRINTABLE_ASCII_CHARS_LEN = sizeof (PRINTABLE_ASCII_CHARS) - 1;

    Job job;

    Status status = schedr_job_set_name(&job, PRINTABLE_ASCII_CHARS, PRINTABLE_ASCII_CHARS_LEN);

    ssct_assert_equals(status, SCHEDR_SUCCESS);
    ssct_assert_equals(job.name, strlen(job.name), PRINTABLE_ASCII_CHARS, PRINTABLE_ASCII_CHARS_LEN);
}

static void set_name_should_return_invalid_argument_error_when_name_argument_contains_non_printable_ascii_symbols()
{
    static const char FIRST_NON_PRINTABLE_ASCII_CHAR = '\x1';
    static const char FIRST_PRINTABLE_ASCII_CHAR = ' ';
    static const int BUFFER_SIZE = 2;

    for (char non_printable = FIRST_NON_PRINTABLE_ASCII_CHAR; non_printable < FIRST_PRINTABLE_ASCII_CHAR; non_printable++)
    {
        char buffer[BUFFER_SIZE];
        buffer[0] = non_printable;
        buffer[BUFFER_SIZE - 1] = '\0';
        Job job;

        Status status = schedr_job_set_name(&job, buffer, BUFFER_SIZE - 1);

        if (status != SCHEDR_ERROR_INVALID_ARGUMENT)
        {
            ssct_fail();
        }
    }
}

static void set_name_should_return_invalid_argument_error_when_name_len_argument_is_equal_to_zero()
{
    Job job;

    Status status = schedr_job_set_name(&job, "hello", 0);

    ssct_assert_equals(status, SCHEDR_ERROR_INVALID_ARGUMENT);
}

static void set_name_should_return_null_argument_error_when_job_argument_is_null()
{
    const char *null_job_name = "null job";

    Status status = schedr_job_set_name(NULL, null_job_name, sizeof (null_job_name) - 1);

    ssct_assert_equals(status, SCHEDR_ERROR_NULL_ARGUMENT);
}

static void set_name_should_return_null_argument_error_when_name_argument_is_null()
{
    Job job;

    Status status = schedr_job_set_name(&job, NULL, sizeof(job.name) - 1);

    ssct_assert_equals(status, SCHEDR_ERROR_NULL_ARGUMENT);
}

static void set_name_should_set_entire_name_member_when_name_argument_length_is_equal_to_max_allowed()
{
    static const char STR_100_RND_CHARS[] = "L2jX0CMU2W39t4Lt8eYZYZmTniyiySdhRTKQyyZpgvlmdLeIoFJWyIlPdZjE8fUllpdZPgM5sRZ0S4ormacyIEe27xrh9WbgiPcE";
    static const int STR_100_RND_CHARS_SIZE = sizeof (STR_100_RND_CHARS) - 1;

    Job job;

    Status status = schedr_job_set_name(&job, STR_100_RND_CHARS, STR_100_RND_CHARS_SIZE);

    ssct_assert_equals(job.name, strlen(job.name), STR_100_RND_CHARS, STR_100_RND_CHARS_SIZE);
    ssct_assert_equals(status, SCHEDR_SUCCESS);
}

static void set_name_should_return_buffer_overflow_error_when_name_argument_length_is_longer_than_max_allowed()
{
    static const char STR_101_RND_CHARS[] = "ysrCek0ygdDVOZ7TTfxkAorGfER1YJEQ0zsUvhAILtgN9mI3hfDiS3GQaOqOlXJQRzV4pPVDBYtSiKuPXh5o6GDA0xP5Q4wuBSWQe";
    static const int STR_101_RND_CHARS_SIZE = sizeof(STR_101_RND_CHARS) - 1;

    Job job;

    Status status = schedr_job_set_name(&job, STR_101_RND_CHARS, STR_101_RND_CHARS_SIZE);

    ssct_assert_equals(status, SCHEDR_ERROR_BUFFER_OVERFLOW);
}

static void set_name_should_return_invalid_argument_error_when_name_argument_is_an_empty_string()
{
    Job job;

    Status status = schedr_job_set_name(&job, "", 0);

    ssct_assert_equals(status, SCHEDR_ERROR_INVALID_ARGUMENT);
}

static void set_command_should_set_command_member_when_command_argument_is_a_valid_command()
{
    static const char VALID_CMD[] = "echo hello";
    static const int VALID_CMD_LEN = sizeof(VALID_CMD) - 1;

    Job job;

    Status status = schedr_job_set_command(&job, VALID_CMD, VALID_CMD_LEN);

    ssct_assert_equals(status, SCHEDR_SUCCESS);
}

static void set_command_should_return_null_argument_error_when_job_argument_is_null()
{
    static const char NULL_JOB_CMD[] = "echo 'hello'";

    Status status = schedr_job_set_command(NULL, NULL_JOB_CMD, sizeof (NULL_JOB_CMD) - 1);

    ssct_assert_equals(status, SCHEDR_ERROR_NULL_ARGUMENT);
}

static void set_command_should_return_null_argument_error_when_command_argument_is_null()
{
    Job job;

    Status status = schedr_job_set_command(&job, NULL, 0);

    ssct_assert_equals(status, SCHEDR_ERROR_NULL_ARGUMENT);
}

static void set_command_should_return_invalid_argument_error_when_command_argument_is_an_empty_string()
{
    Job job;

    Status status = schedr_job_set_command(&job, "", 0);

    ssct_assert_equals(status, SCHEDR_ERROR_INVALID_ARGUMENT);
}

static void set_command_should_return_invalid_argument_error_when_cmd_len_argument_is_equal_to_zero()
{
    Job job;

    Status status = schedr_job_set_command(&job, "hello", 0);

    ssct_assert_equals(status, SCHEDR_ERROR_INVALID_ARGUMENT);
}

static void set_command_should_set_entire_command_member_when_command_argument_length_is_equal_to_max_allowed()
{
    static const char CMD_1000_CHARS[] = "echo 'HAbkJxzWH1OXvYzFlnCvPHvd2LbICMcIl8E9JwbbMGiAIwm2q1UGV91apjORHy9dM4NhFTxXmY9gL858pivwzEU1M1wRSEiNqyGe7p3Hjk3NfRUN0SdRISEkQZUWSphSFHrjQCPLMQrxapzsr8KLU95rzzCADyp28fONouI6ke8Hk6lHPImegBWHwZ4hFqVyASyfIpatfUWzafRIIVABkwF6diPFBl9RbFaocyHslWddgQEJTiMY3tjMm5kzNisqqw5t1KVagFMP2I3814Bv1iHjXevXt5oI65UFWbxKnItbFRfQ98f3kjP3Tiwl8BsJ6CGUz02GrmDc6sueTJoy1I35RWWWe0Ud0y4nW8134qsi5g26yO8g05hLyPF0TJ3uL4hArvE21doiLyATeuFkzgu5tTydNgjMEvprMMRAgmRvc4Cm0Lq7UCJCdk7C2Ox5PBRdQfFOlcJINHcPsV5GQPkuwqfplhes6s6zC2dx7F5suuEGAM0iEavjrFM24DxVmoeg2wU9D5F2XvWRDiHIgV6lFnnw3GbGdnFHwHOlS07bRxoDUzaKuaSizF55j2CmIJ6sMgu84l0AQZro7EyXmmYtUH5N4krgPlV1ZYwChVHiL14zYe3xkgbJA0aZ5Hr4ude8bEidVU7wWff7sbgr9SjzYj8zRBtRx8XaNcupgivf6fVj9cFHxzkvTL24IhZeVC07eqa4VzBGgOvd6plMG2lF4JUwTxawNfYxO3HWooumA2OwfVGlObE9KxNAGYshFpptJ5fDOAhmz0WRFVdBWFkWwtmxm80IWN2fOWM0UvgYu4O3booYAXhhv1eM6QMBb3aIUuKMyGSZzjtUMAPlQeiHUc7d3sR9YA4uynmYHNOoi5zrZLBppFRgkmtk2H02XVQUEH3Xn1S3xqbhshETzs3Cc03sPtBtm03LVnNBW9E9RdkJCu59OsFWrTpZGOYExLEfqi0WFBegL4dipZPzUHmiaNKorE7m9'";
    static const int CMD_1000_CHARS_LEN = sizeof (CMD_1000_CHARS) - 1;

    Job job;

    Status status = schedr_job_set_command(&job, CMD_1000_CHARS, CMD_1000_CHARS_LEN);

    ssct_assert_equals(job.command, strlen(job.command), CMD_1000_CHARS, CMD_1000_CHARS_LEN);
    ssct_assert_equals(status, SCHEDR_SUCCESS);
}

static void set_command_should_return_buffer_overflow_error_when_command_argument_length_is_longer_than_max_allowed()
{
    static const char CMD_1001_CHARS[] = "echo 'HAb6kJxzWH1OXvYzFlnCvPHvd2LbICMcIl8E9JwbbMGiAIwm2q1UGV91apjORHy9dM4NhFTxXmY9gL858pivwzEU1M1wRSEiNqyGe7p3Hjk3NfRUN0SdRISEkQZUWSphSFHrjQCPLMQrxapzsr8KLU95rzzCADyp28fONouI6ke8Hk6lHPImegBWHwZ4hFqVyASyfIpatfUWzafRIIVABkwF6diPFBl9RbFaocyHslWddgQEJTiMY3tjMm5kzNisqqw5t1KVagFMP2I3814Bv1iHjXevXt5oI65UFWbxKnItbFRfQ98f3kjP3Tiwl8BsJ6CGUz02GrmDc6sueTJoy1I35RWWWe0Ud0y4nW8134qsi5g26yO8g05hLyPF0TJ3uL4hArvE21doiLyATeuFkzgu5tTydNgjMEvprMMRAgmRvc4Cm0Lq7UCJCdk7C2Ox5PBRdQfFOlcJINHcPsV5GQPkuwqfplhes6s6zC2dx7F5suuEGAM0iEavjrFM24DxVmoeg2wU9D5F2XvWRDiHIgV6lFnnw3GbGdnFHwHOlS07bRxoDUzaKuaSizF55j2CmIJ6sMgu84l0AQZro7EyXmmYtUH5N4krgPlV1ZYwChVHiL14zYe3xkgbJA0aZ5Hr4ude8bEidVU7wWff7sbgr9SjzYj8zRBtRx8XaNcupgivf6fVj9cFHxzkvTL24IhZeVC07eqa4VzBGgOvd6plMG2lF4JUwTxawNfYxO3HWooumA2OwfVGlObE9KxNAGYshFpptJ5fDOAhmz0WRFVdBWFkWwtmxm80IWN2fOWM0UvgYu4O3booYAXhhv1eM6QMBb3aIUuKMyGSZzjtUMAPlQeiHUc7d3sR9YA4uynmYHNOoi5zrZLBppFRgkmtk2H02XVQUEH3Xn1S3xqbhshETzs3Cc03sPtBtm03LVnNBW9E9RdkJCu59OsFWrTpZGOYExLEfqi0WFBegL4dipZPzUHmiaNKorE7m9'";

    Job job;

    Status status = schedr_job_set_command(&job, CMD_1001_CHARS, sizeof (CMD_1001_CHARS) - 1);

    ssct_assert_equals(status, SCHEDR_ERROR_BUFFER_OVERFLOW);
}

static void set_interval_should_return_null_argument_error_when_job_argument_is_null()
{
    Status status = schedr_job_set_interval(NULL, 1);

    ssct_assert_equals(status, SCHEDR_ERROR_NULL_ARGUMENT);
}

static void set_interval_should_return_invalid_argument_error_when_interval_argument_is_negative()
{
    Job job;

    Status status = schedr_job_set_interval(&job, -1);

    ssct_assert_equals(status, SCHEDR_ERROR_INVALID_ARGUMENT);
}

static void set_state_should_return_null_argument_error_when_job_argument_is_null()
{
    Status status = schedr_job_set_state(NULL, Stopped);

    ssct_assert_equals(status, SCHEDR_ERROR_NULL_ARGUMENT);
}

static void set_state_should_return_invalid_argument_error_when_state_argument_is_negative()
{
    Job job;

    Status status = schedr_job_set_state(&job, -1);

    ssct_assert_equals(status, SCHEDR_ERROR_INVALID_ARGUMENT);
}

static void set_state_should_return_invalid_argument_error_when_state_argument_is_out_of_range()
{
    Job job;

    Status status = schedr_job_set_state(&job, SCHEDR_JOB_STATE_VALUES);

    ssct_assert_equals(status, SCHEDR_ERROR_INVALID_ARGUMENT);
}

