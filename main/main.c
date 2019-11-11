/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2019 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This is the main entry point for the application.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_MAIN            ( "ALLOCATE STORAGE FOR MAIN" )

/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <string.h>             //  Functions for managing strings
#include <stdlib.h>             //  ANSI standard library.
#include <unistd.h>             //  UNIX standard library.
#include <ctype.h>              //  Testing and mapping characters.
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <main_api.h>           //  Global stuff for this application
#include <libtools_api.h>       //  My Tools Library
                                //*******************************************
//#include <decode_api.h>         //  API for all decode_*            PUBLIC
                                //*******************************************

/****************************************************************************
 * API Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
enum    decode_state_e
{
    DS_IDLE                 =   0,
    DS_FROM                 =   1,
    DS_TAG_1                =   2,
    DS_TAG_2                =   3,
    DS_TAG_3                =   4,
    DS_EMAIL                =   5,
    DS_EMAIL_BODY           =   6,
    DS_NEW_TAG_1            =   7,
    DS_NEW_TAG_2            =   8,
    DS_NEW_TAG_3            =   9,
    DS_NEW_EMAIL            =  10,
    DS_END                  =  99
};
//----------------------------------------------------------------------------

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
#define NO_IF_OR_ID             ( 1 )
#define BOTH_IF_AND_ID          ( 2 )
//----------------------------------------------------------------------------
#define SOURCE_L                ( 1024 )
#define FROM_L                  ( 1024 )
#define DATETIME_L              ( 1024 )
#define SUBJECT_L               ( 1024 )
//----------------------------------------------------------------------------
#define MIN_TAG_L               ( 4 )
//----------------------------------------------------------------------------

/****************************************************************************
 * Private Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
struct  source_info_t
{
    //------------------------------------------------------------------------
    /**
     *  @param  e_source        Group-From                                  */
    char                            g_from[      FROM_L      + 1 ];
    /**
     *  @param  e_subject       Group_Subject                               */
    char                            g_subject[   SUBJECT_L   + 1 ];
    /**
     *  @param  e_datetime      Group sent Date/Time                        */
    char                            g_datetime[  DATETIME_L  + 1 ];
    //------------------------------------------------------------------------
    /**
     *  @param  e_from          Who posted the e-Mail message               */
    char                            e_from[      FROM_L      + 1 ];
    /**
     *  @param  e_subject       Subject of the original e-Mail              */
    char                            e_subject[   SUBJECT_L   + 1 ];
    /**
     *  @param  e_datetime      When the e-Mail was originally posted       */
    char                            e_datetime[  DATETIME_L  + 1 ];
    //------------------------------------------------------------------------
    /**
     *  @param  f_dir_name      Directory the file was when it was decoded  */
    char                            f_dir_name[  FILE_NAME_L + 1 ];
    /**
     *  @param  e_file_name     The file name that was decoded              */
    char                            f_file_name[ FILE_NAME_L + 1 ];
    /**
     *  @param  f_date_time     Date & Time stamp of the file               */
    char                            f_date_time[ FILE_DATE_L + 1 ];
    /**
     *  @param  f_size          Size of the file that was decoded           */
    char                            f_file_size[ FILE_SIZE_L + 1 ];
};
//----------------------------------------------------------------------------

/****************************************************************************
 * Private Storage Allocation
 ****************************************************************************/

//----------------------------------------------------------------------------
/**
 * @param in_file_name_p    Pointer to the input file name                  */
char                        *   in_file_name_p;
//----------------------------------------------------------------------------
/**
 * @param in_dir_name_p     Pointer to the input directory name             */
char                        *   in_dir_name_p;
//----------------------------------------------------------------------------

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************/
/**
 *  Scan the command line and extract parameters for the application.
 *
 *  @param  argc                Number of command line parameters.
 *  @param  argv                Indexed list of command line parameters
 *
 *  @return main_rc             TRUE when all parameters are successfully
 *                              processed, else FALSE is returned
 *
 *  @note
 *
 ****************************************************************************/

static
void
help(
    int                             error_id
    )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    switch( error_id )
    {
        case    NO_IF_OR_ID:
        {
            log_write( MID_INFO, "main: help",
                          "Missing -if or -id     "
                          "Did not find a file name or directory name that "
                          "is to be scanned.\n" );
        }   break;
        case    BOTH_IF_AND_ID:
        {
            log_write( MID_INFO, "main: help",
                          "Both -if and -id found "
                          "Only one of the two may be used.\n" );
        }   break;
    }

    //  Command line options
    log_write( MID_INFO, "main: help",
                  "-if {file_name}          Input file name\n" );
    log_write( MID_INFO, "main: help",
                  "-id {directory_name}     Input directory name\n" );
    log_write( MID_FATAL, "main: help",
                  "-od {directory_name}     Output directory name\n" );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the command line and extract parameters for the application.
 *
 *  @param  argc                Number of command line parameters.
 *  @param  argv                Indexed list of command line parameters
 *
 *  @return void                Nothing is returned from this function
 *
 *  @note
 *
 ****************************************************************************/

static
void
command_line(
    int                             argc,
    char                        *   argv[]
      )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the pointers
    in_file_name_p = NULL;
    in_dir_name_p  = NULL;
    out_dir_name_p = NULL;

    /************************************************************************
     *  Scan for parameters
     ************************************************************************/

    //  Scan for        Input File name
    in_file_name_p = get_cmd_line_parm( argc, argv, "if" );

    //  Scan for        Input Directory name
    in_dir_name_p = get_cmd_line_parm( argc, argv, "id" );

    //  Scan for        Input Directory name
    out_dir_name_p = get_cmd_line_parm( argc, argv, "od" );

    //  DEBUG DEFAULTS
    if (    ( in_file_name_p       == NULL )
         && ( in_dir_name_p        == NULL ) )
    {
        in_dir_name_p        = "/home/greg/work/RecipeSourceFiles";
        out_dir_name_p       = "/home/greg/work/RecipeOutputFiles";
    }

    /************************************************************************
     *  Verify everything is good
     ************************************************************************/

    //  Is there an Input File name or an Input Directory name ?
    if (    ( in_file_name_p == NULL )
         && ( in_dir_name_p  == NULL ) )
    {
        //  NO:     Write some help information
        help( NO_IF_OR_ID );
    }

    //  Is there both an Input File name and an Input Directory name ?
    if (    ( in_file_name_p != NULL )
         && ( in_dir_name_p  != NULL ) )
    {
        //  YES:    Write some help information
        help( BOTH_IF_AND_ID );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Look at the text string to see if it is a valid 'From ' input line.
 *
 *  @param  argc                Number of command line parameters.
 *  @param  argv                Indexed list of command line parameters
 *
 *  @return is_from_rc          TRUE when the input line is a valid 'From '
 *                              line; else FALSE is returned.
 *
 *  @note
 *
 ****************************************************************************/

static
int
is_from(
    char                        *   data_p
    )
{
    /**
     * @param main_rc           Return code from called functions.          */
    int                             is_from_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT 'From ' input line.
    is_from_rc = false;

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Does the line begin with 'From '
    if ( strncmp( data_p, "From ", 5 ) == 0 )
    {
        //  NO:     Set the return code.
        is_from_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
        return( is_from_rc );
}

/****************************************************************************/
/**
 *  Look at the text string to see if it is a valid e-mail tag.
 *
 *  @param  argc                Number of command line parameters.
 *  @param  argv                Indexed list of command line parameters
 *
 *  @return is_tag_rc           TRUE when the input line is a valid e-mail
 *                              tag; else FALSE is returned.
 *
 *  @note
 *      Valid e-mail tags are a string of alpha and hyphen [-] characters
 *      followed by a colon [:] and space [ ] characters.
 *
 ****************************************************************************/

static
int
is_tag(
    char                        *   data_p
    )
{
    /**
     * @param tag_rc            Return code from called functions.          */
    int                             is_tag_rc;
    /**
     * @param ndx               Index into the input line.                  */
    int                             ndx;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT 'From ' input line.
    is_tag_rc = false;

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Start looking at the input line character-by-character.
    for ( ndx = 0;
          ndx < strlen( data_p );
          ndx += 1 )
    {
        //  Is this a colon [:] character more then MIN_TAG_L from the start ?
        if (    ( data_p[ ndx ] == ':'       )
             && (         ndx   <  MIN_TAG_L ) )
        {
            //  NO:     Not a valid e-mail tag
            break;
        }
        else
        {
            //  Is this character valid for an e-mail tag ?
            if (    ( isalpha( data_p[ ndx ] ) !=  0  )
                 || (          data_p[ ndx ]   == ' ' )
                 || (          data_p[ ndx ]   == ':' )
                 || (          data_p[ ndx ]   == '-' ) )
            {
                //  YES:    Is this the end of a valid e-mail tag ?
                if ( data_p[ ndx ] == ' ' )
                {
                    //  YES:    This is a valid e-mail tag.
                    is_tag_rc = true;

                    //  We are done here
                    break;
                }
            }
            else
            {
                //  NO:     Not a valid character
                break;
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( is_tag_rc );
}

/****************************************************************************/
/**
 *  Look at the text string to see if it is a valid e-mail tag.
 *
 *  @param  argc                Number of command line parameters.
 *  @param  argv                Indexed list of command line parameters
 *
 *  @return is_tag_rc           TRUE when the input line is a valid e-mail
 *                              tag; else FALSE is returned.
 *
 *  @note
 *      Valid e-mail tags are a string of alpha and hyphen [-] characters
 *      followed by a colon [:] and space [ ] characters.
 *
 ****************************************************************************/

static
FILE    *
open_output_file (
    void
    )
{
    /**
     * @param file_num_p        Pointer to the file name.                   */
    char                        *   file_name_p;
    /**
     * @param next_num_p        Pointer to the next file name.              */
    char                        *   next_name_p;
    /**
     *  @param  file_num_val    Value of the stored file number             */
    int                             file_num_val;
    /**
     * @param out_name          Encoded output file name                    */
    char                            out_name[ ( FILE_NAME_L * 3 ) ];
    /**
     * @param input_file_fp     Output File pointer                         */
    FILE                        *   out_file_fp;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the file pointer
    out_file_fp = NULL;

    //  Clean out the fully qualified file name buffer
    memset( out_name, '\0', ( FILE_NAME_L * 3 ) );

    /************************************************************************
     *  Get the current file number increment it and save it.
     ************************************************************************/

    //  Put the current file number.
    file_name_p = store_get( "FILE_NUM" );

    //  Allocate storage for the next file number
    next_name_p = mem_malloc( strlen( file_name_p + 1 ) );

    //  Success ?
    if ( file_name_p == NULL )
    {
        //  NO:     OOPS, this shouldn't happen.
        log_write( MID_FATAL, "open_output_file",
                "Unable to get the current file number." );
    }

    //  Convert the recipe number string to an integer
    file_num_val = atoi( file_name_p );

    //  And increment it
    file_num_val += 1;

    //  Back to a string
    snprintf( next_name_p, ( strlen( file_name_p ) + 1 ), "%016d", file_num_val );

    //  Put the incremented file number which is now the current file number.
    store_put( "FILE_NUM", next_name_p );

    /************************************************************************
     *  Build the directory/file name and create directories when needed.
     ************************************************************************/

    //  Start building the output name
    snprintf( out_name, sizeof( out_name ), "%s", out_dir_name_p );

    //  If the directory does not already exist, create it.
    file_dir_exist( out_name, true );

    //  Build the fully qualified file name.
    snprintf( out_name, sizeof( out_name ),
              "%s/%s.txt", out_dir_name_p, file_name_p );

    /************************************************************************
     *  Open the file for write
     ************************************************************************/

    //  Open the input file
    out_file_fp = file_open_write( out_name );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Release the allocated storage.
    mem_free( next_name_p );

    //  DONE!
    return( out_file_fp );
}

/****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************/
/**
 *  This is the main entry point for the application.
 *
 *  @param  argc                Number of command line parameters.
 *  @param  argv                Indexed list of command line parameters
 *
 *  @return                     Zero for success. Any other value is an error.
 *
 *  @note
 *
 ****************************************************************************/

int
main(
    int                             argc,
    char                        *   argv[ ]
    )
{
    /**
     * @param main_rc           Return code for the overall application.    */
    enum    queue_rc_e              main_rc;
    /**
     * @param decode_state      State of the file decoder.                  */
    enum    decode_state_e          decode_state;
    /**
     *  @param  file_list       Pointer to a list of files                  */
    struct  list_base_t         *   file_list_p;
    /**
     *  @param  file_info_p     Pointer to a file information structure     */
    struct  file_info_t         *   file_info_p;
    /**
     *  @param  input_file_name Buffer to hold the directory/file name      */
    char                            input_file_name[ ( FILE_NAME_L * 3 ) ];
    /**
     * @param input_file_fp     Input File pointer                          */
    FILE                        *   in_file_fp;
    /**
     * @param source_info_p     Pointer to source data information          */
    struct  source_info_t       *   source_info_p;
    /**
     * @param read_data_p       Pointer to the raw read data                */
    char                        *   read_data_p;
    /**
     * @param input_file_fp     Output File pointer                         */
    FILE                        *   out_file_fp;
    /**
     *  @param  from_data_p     Pointer to the 'From ' line.                */
    char                        *   from_data_p;
    /**
     *  @param  tag_1_data_p    Pointer to the first e-mail tag line.       */
    char                        *   tag_1_data_p;
    /**
     *  @param  tag_2_data_p    Pointer to the second e-mail tag line.      */
    char                        *   tag_2_data_p;
    /**
     *  @param  tag_3_data_p    Pointer to the third e-mail tag line.       */
    char                        *   tag_3_data_p;

    /************************************************************************
     *  Application Initialization
     ************************************************************************/

    //  Initialize the CommonCode memory process
    token_init( );
    mem_init( );
    store_init( );

    //  Nothing is open yet
    out_file_fp = NULL;

    //  Set the starting decode state
    decode_state = DS_IDLE;
    
    //  Allocate a data buffer for the read data.
    from_data_p = mem_malloc( MAX_LINE_L );

    /************************************************************************
     *  Initialize the File-Num:
     ************************************************************************/

    //  Until a file_num is available
    do
    {
        /**
         *  @param  recipe_id       Temporary data buffer for the recipe id */
        unsigned char                   file_num[ SHA1_DIGEST_SIZE + 2 ];

        //  Get the current recipe-id.
        recipe_id_p = store_get( "FILE_NUM" );

        //  Success ?
        if ( recipe_id_p == NULL )
        {
            //  NO:     Create it.
            snprintf( (char*)file_num, sizeof( file_num ), "%016d", 0 );

            //  Put the new recipe-id
            store_put( "FILE_NUM", (char*)file_num );
        }

    }   while( recipe_id_p == NULL );

    /************************************************************************
     *  Initialize the log process
     ************************************************************************/

    //  Initialize the log handler
    log_init( "Mbox2Txt.log" );

    //  Mark the start of the run in the log file
    log_write( MID_INFO, "main",
                  "|==============================================|\n" );
    log_write( MID_INFO, "main",
                  "Starting: Recipe Import Assist Version %s %s.\n",
                  __DATE__, __TIME__ );

    for ( int count = 0;
              count < argc;
              count ++ )
    {
        log_write( MID_INFO, "main", "arcv[ %d ]: '%s'\n", count, argv[ count ] );
    }

    //  Log the event
    log_write( MID_INFO, "main",
                  "Log initialization complete.\n" );

    /************************************************************************
     *  Command line processing
     ************************************************************************/

    //  Process the command line parameters
    command_line( argc, argv );

    //  Create the file-list
    file_list_p = list_new( );

    //  Are we processing a directory ?
    if ( in_dir_name_p != NULL )
    {
        //  Unzip all "*.zip" files
        file_unzip( in_dir_name_p );

        //  YES:    Build the file list
        file_ls( file_list_p, in_dir_name_p, NULL );
    }
    else
    {
        //  Get the file information for the input file
        file_info_p = file_stat( in_file_name_p, in_file_name_p );

        //  Add the new file to the list
        list_put_last( file_list_p, file_info_p );
    }

    /************************************************************************
     *  The application processing starts here:
     ************************************************************************/

    //  Scan the list
    for( file_info_p = list_get_first( file_list_p );
         file_info_p != NULL;
         file_info_p = list_get_next( file_list_p, file_info_p ) )
    {
        //  Remove it from the list
        list_delete( file_list_p, file_info_p );

        //  Will the fully qualified file name will fit in the buffer ?
        if (     (   ( strlen( file_info_p->dir_name  ) )
                   + ( strlen( file_info_p->file_name ) ) )
              >= ( sizeof( input_file_name ) ) )
        {
            //  NO:     This is bad..
            log_write( MID_WARNING, "main",
                          "The file name is too big for the buffer provided. \n" );
            log_write( MID_FATAL, "main",
                          "'%s/%s'\n",
                          file_info_p->dir_name, file_info_p->file_name );
        }
        else
        {
            //  YES:    build the full file name.
            snprintf( input_file_name, sizeof( input_file_name ) - 1,
                       "%s/%s",
                       file_info_p->dir_name, file_info_p->file_name );
        }

        //  Open the input file
        in_file_fp = file_open_read( input_file_name );

        //  Is there an output file opened ?
        if ( out_file_fp != NULL )
        {
            //  YES:    Close it
            file_close( out_file_fp );
        }

        //  Open a new output file.
        out_file_fp = open_output_file( );

        //  Log the event
        log_write( MID_INFO, "main",
                   "Working on file: '%s'\n", input_file_name );

        //  Create a new source information structure
        source_info_p = mem_malloc( sizeof( struct source_info_t ) );

        log_write( MID_DEBUG_1, "main.c", "Line: %d\n", __LINE__ );

        //  Copy file information into source information structure
        strncpy( source_info_p->f_dir_name, file_info_p->dir_name,
                 sizeof( source_info_p->f_dir_name ) );
        strncpy( source_info_p->f_file_name, file_info_p->file_name,
                 sizeof( source_info_p->f_file_name ) );
        strncpy( source_info_p->f_date_time, file_info_p->date_time,
                 sizeof( source_info_p->f_date_time ) );
        strncpy( source_info_p->f_file_size, file_info_p->file_size,
                 sizeof( source_info_p->f_file_size ) );

        /********************************************************************
         *  Process the file
         ********************************************************************/
        do
        {
            //  Read another line from the file
            read_data_p = file_read_text( in_file_fp, 0 );

            //  Was the read successful ?
            if (    ( read_data_p != END_OF_FILE )
                 && ( read_data_p != NULL        ) )
            {
                //  YES:    Start decoding the input file
                switch ( decode_state )
                {
                //  ########
                case        DS_IDLE:
                {
                    //  Is the current input line a valid 'From ' line ?
                    if ( is_from( read_data_p ) == true )
                    {
                        //  YES:    Initialize the tag data pointers
                        tag_1_data_p = NULL;
                        tag_2_data_p = NULL;
                        tag_3_data_p = NULL;

                        //  Save the 'From ' line text
                        memset( from_data_p, '\0', MAX_LINE_L );
                        memcpy( from_data_p, read_data_p, strlen( read_data_p ) );

                        //  Set the next state.
                        decode_state = DS_TAG_1;
                    }
                }   break;
                //  ########
                case        DS_TAG_1:
                {
                    //  Is the current input line a valid e-mail tag ?
                    if ( is_tag( read_data_p ) == true )
                    {
                        //  YES:    Save the input line
                        tag_1_data_p = text_copy_to_new( read_data_p );

                        //  Set the next state.
                        decode_state = DS_TAG_2;
                    }
                    else
                    {
                        //  NO:     Start over looking for a 'From ' line.
                        decode_state = DS_IDLE;
                    }
                }   break;
                //  ########
                case        DS_TAG_2:
                {
                    //  Is the current input line a valid e-mail tag ?
                    if ( is_tag( read_data_p ) == true )
                    {
                        //  YES:    Save the input line
                        tag_2_data_p = text_copy_to_new( read_data_p );

                        //  Set the next state.
                        decode_state = DS_TAG_3;
                    }
                    else
                    {
                        //  NO:     Start over looking for a 'From ' line.
                        decode_state = DS_IDLE;
                    }
                }   break;
                //  ########
                case        DS_TAG_3:
                {
                    //  Is the current input line a valid e-mail tag ?
                    if ( is_tag( read_data_p ) == true )
                    {
                        //  YES:    Save the input line
                        tag_3_data_p = text_copy_to_new( read_data_p );

                        //  Set the next state.
                        decode_state = DS_EMAIL;
                    }
                    else
                    {
                        //  NO:     Start over looking for a 'From ' line.
                        decode_state = DS_IDLE;
                    }
                }   break;
                //  ########
                case        DS_EMAIL:
                {
                    //  Is the current input line a valid e-mail tag ?
                    if ( is_tag( read_data_p ) == true )
                    {
                        //  YES:    Modify the 'From ' string to 'From - '
                        text_insert( from_data_p, MAX_LINE_L, 4, " -" );

                        //  Log the new e-mail
                        log_write( MID_INFO, "main", "%s'\n", from_data_p );

                        //  Write the saved data to the file
                        fprintf( out_file_fp, "%s\n", from_data_p  );
                        fprintf( out_file_fp, "%s\n", tag_1_data_p );
                        fprintf( out_file_fp, "%s\n", tag_2_data_p );
                        fprintf( out_file_fp, "%s\n", tag_3_data_p );
                        fprintf( out_file_fp, "%s\n", read_data_p  );

                        //  Free storage for the tag buffers.
                        mem_free( tag_1_data_p );
                        mem_free( tag_2_data_p );
                        mem_free( tag_3_data_p );

                        //  Set the next state.
                        decode_state = DS_EMAIL_BODY;
                    }
                }   break;
                //  ########
                case        DS_EMAIL_BODY:
                {
                    //  Is the current input line a valid 'From ' line ?
                    if ( is_from( read_data_p ) == false )
                    {
                        //  NO:     Just write it to the open output file.
                        fprintf( out_file_fp, "%s\n", read_data_p  );
                    }
                    else
                    {
                        //  YES:    Initialize the tag data pointers
                        tag_1_data_p = NULL;
                        tag_2_data_p = NULL;
                        tag_3_data_p = NULL;

                        //  Save the 'From ' line text
                        memset( from_data_p, '\0', MAX_LINE_L );
                        memcpy( from_data_p, read_data_p, strlen( read_data_p ) );

                        //  Set the next state.
                        decode_state = DS_NEW_TAG_1;
                    }
                }   break;
                //  ########
                case        DS_NEW_TAG_1:
                {
                    //  Is the current input line a valid e-mail tag ?
                    if ( is_tag( read_data_p ) == true )
                    {
                        //  YES:    Save the input line
                        tag_1_data_p = text_copy_to_new( read_data_p );

                        //  Set the next state.
                        decode_state = DS_NEW_TAG_2;
                    }
                    else
                    {
                        //  NO:     Not a new e-mail message.  Save the
                        //          buffered lines.
                        fprintf( out_file_fp, "%s\n", from_data_p  );
                        fprintf( out_file_fp, "%s\n", read_data_p  );

                        //  Continue with the current e-mail
                        decode_state = DS_EMAIL_BODY;
                    }
                }   break;
                //  ########
                case        DS_NEW_TAG_2:
                {
                    //  Is the current input line a valid e-mail tag ?
                    if ( is_tag( read_data_p ) == true )
                    {
                        //  YES:    Save the input line
                        tag_2_data_p = text_copy_to_new( read_data_p );

                        //  Set the next state.
                        decode_state = DS_NEW_TAG_3;
                    }
                    else
                    {
                        //  NO:     Not a new e-mail message.  Save the
                        //          buffered lines.
                        fprintf( out_file_fp, "%s\n", from_data_p  );
                        fprintf( out_file_fp, "%s\n", tag_1_data_p );
                        fprintf( out_file_fp, "%s\n", read_data_p  );

                        //  Continue with the current e-mail
                        decode_state = DS_EMAIL_BODY;
                    }
                }   break;
                //  ########
                case        DS_NEW_TAG_3:
                {
                    //  Is the current input line a valid e-mail tag ?
                    if ( is_tag( read_data_p ) == true )
                    {
                        //  YES:    Save the input line
                        tag_3_data_p = text_copy_to_new( read_data_p );

                        //  Set the next state.
                        decode_state = DS_NEW_EMAIL;
                    }
                    else
                    {
                        //  NO:     Not a new e-mail message.  Save the
                        //          buffered lines.
                        fprintf( out_file_fp, "%s\n", from_data_p  );
                        fprintf( out_file_fp, "%s\n", tag_1_data_p );
                        fprintf( out_file_fp, "%s\n", tag_2_data_p );
                        fprintf( out_file_fp, "%s\n", read_data_p  );

                        //  Continue with the current e-mail
                        decode_state = DS_EMAIL_BODY;
                    }
                }   break;
                //  ########
                case        DS_NEW_EMAIL:
                {
                    //  Is the current input line a valid e-mail tag ?
                    if ( is_tag( read_data_p ) == true )
                    {
                        //  YES:    Modify the 'From ' string to 'From - '
                        text_insert( from_data_p, MAX_LINE_L, 4, " -" );

                        //  Log the new e-mail
                        log_write( MID_INFO, "main", "%s'\n", from_data_p );

                        //  Write the saved data to the file
                        fprintf( out_file_fp, "%s\n", from_data_p  );
                        fprintf( out_file_fp, "%s\n", tag_1_data_p );
                        fprintf( out_file_fp, "%s\n", tag_2_data_p );
                        fprintf( out_file_fp, "%s\n", tag_3_data_p );
                        fprintf( out_file_fp, "%s\n", read_data_p  );

                        //  Free storage for the tag buffers.
                        mem_free( tag_1_data_p );
                        mem_free( tag_2_data_p );
                        mem_free( tag_3_data_p );

                        //  Set the next state.
                        decode_state = DS_EMAIL_BODY;
                    }
                }   break;
                //  ########
                default:
                {
                    //  OOPS!   We should never get here
                    log_write( MID_FATAL, "main",
                               "Invalid decode state [%d] detected.\n",
                               decode_state );
                }
                }
            }

        }   while( read_data_p != END_OF_FILE );
    }

    /************************************************************************
     *  Application Exit
     ************************************************************************/

    //  Mark the end of the run in the log file
    log_write( MID_INFO, "main",
                  "End\n" );
    log_write( MID_INFO, "main",
                  "|==============================================|\n\n" );

    return( 0 );
}

/****************************************************************************/
