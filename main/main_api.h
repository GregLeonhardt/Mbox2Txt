/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

#ifndef MAIN_API_H
#define MAIN_API_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public definitions (etc.) that are accessible to the
 *  entire project.  I.E. "global".
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#ifdef ALLOC_MAIN
   #define MAIN_EXT
#else
   #define MAIN_EXT             extern
#endif

/****************************************************************************
 * System APIs
 ****************************************************************************/

                                //*******************************************
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
                                //*******************************************

/****************************************************************************
 * Library Public Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Storage Allocation
 ****************************************************************************/

//----------------------------------------------------------------------------
/**
 * @param out_dir_name_p        Pointer to the output directory name        */
char                        *   out_dir_name_p;
//---------------------------------------------------------------------------
/**
 *  @param  store_value_p       ID number for the DECODE queue              */
char                        *   recipe_id_p;
//---------------------------------------------------------------------------

/****************************************************************************
 * Library Public Prototypes
 ****************************************************************************/

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    MAIN_API_H