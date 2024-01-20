#! /usr/bin/env python3
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2024  CEA, EDF, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

import os
import shutil
import argparse

# Options of this script
def profileQuickStartParser() :

    parser = argparse.ArgumentParser( usage = "usage: python app-quickstart.py [options]" )

    parser.add_argument('-p',
                      "--prefix",
                      metavar="</Path/to/the/sources/of/application>",
                      action="store",
                      dest="prefix",
                      default='.',
                      help="Where the application's sources will be generated. [Default : '.']")

    parser.add_argument('-m',
                      "--modules",
                      metavar="<module1,module2,...>",
                      action="store",
                      dest="modules",
                      default='KERNEL,GUI',
                      help="List of the application's modules. [Default : KERNEL,GUI]")

    parser.add_argument('-n',
                      "--name",
                      action="store",
                      dest="name",
                      help="Name of the application")

    parser.add_argument('-v',
                      "--version",
                      action="store",
                      dest="version",
                      default='1.0',
                      help="Version of the application. [Default : 1.0]")

    parser.add_argument('-s',
                      "--slogan",
                      action="store",
                      dest="slogan",
                      default='',
                      help="Slogan of the application.")

    parser.add_argument('-f',
                      "--force",
                      action="store_true",
                      dest="force",
                      help="Overwrites existing sources")

    return parser



#Create the splash screen
def profileGenerateSplash( resources_dir, appname, version, subtext ):
    import Image
    import ImageDraw
    import ImageFont

    if isinstance(appname, bytes):
        uname = str(appname, 'UTF-8')
    else:
        uname = appname
    if isinstance(version, bytes):
        uversion = str(version, 'UTF-8')
    else:
        uversion = version

    # fonts
    fontbig = ImageFont.truetype( os.path.join( resources_dir, 'Anita semi square.ttf' ), 64)
    fontsmall = ImageFont.truetype( os.path.join( resources_dir, 'Anita semi square.ttf' ), 20)
    textColor = "rgb(255, 250, 250)"
    shadowColor = "rgb(0, 0, 0)"

    # dimensions
    nbcar = len(uname)
    width = 600
    if nbcar > 12:
        width = min( width*nbcar//12, 1024) #a little more
    height = 300
    borderX = 30 #50
    borderY = 3 #30
    shadowX = 2
    shadowY = shadowX

    # load background image 
    f0 = os.path.join( resources_dir, "background.png" )
    im = Image.open(f0)
    im = im.resize( ( width, height ) )
    draw = ImageDraw.Draw(im)

    # add the name of the application
    iw, ih = draw.textsize(uname, font=fontbig)
    x = (width - iw) / 2.0 # horizontal center
    y = (height - ih) / 2.0 # vertical center
    draw.text((x+shadowX, y+shadowY), uname, font=fontbig, fill=shadowColor)
    draw.text((x, y), uname, font=fontbig, fill=textColor)

    # add subtext
    if len(subtext) > 0:
        iw, ih = draw.textsize(subtext, font=fontsmall)
        draw.text((borderX+shadowX, height+shadowY-borderY-ih),
                  subtext, font=fontsmall, fill=shadowColor)
        draw.text((borderX, height-borderY-ih),
                  subtext, font=fontsmall, fill=textColor)

    # add the version if any
    if len(version) > 0:
        iw, ih = draw.textsize(uversion, font=fontsmall)
        draw.text((width+shadowX-borderX-iw, height+shadowY-borderY-ih),
                  uversion, font=fontsmall, fill=shadowColor)
        draw.text((width-borderX-iw, height-borderY-ih),
                  uversion, font=fontsmall, fill=textColor)

    del draw
    return im


#Create the application logo
def profileGenerateLogo( appname, font ):
    import Image
    import ImageDraw

    if isinstance(appname, bytes):
        uname = str(appname, 'UTF-8')
    else:
        uname = appname

    # evaluate size before deleting draw
    im = Image.new( "RGBA", (1, 1), (0, 0, 0, 0) )
    draw = ImageDraw.Draw( im )

    im = Image.new( "RGBA", draw.textsize( uname, font=font ), (0, 0, 0, 0) )
    draw = ImageDraw.Draw(im)
    draw.text( (0+1, 0), uname, font=font, fill="rgb(0, 0, 0)" )
    draw.text( (0, -1), uname, font=font, fill="rgb(191, 191, 191)" )

    del draw
    return im


# Check if filename is a binary file
def is_binary(filename):
    """ returns True if filename is a binary file
    (from https://stackoverflow.com/a/7392391/2531279)
    """
    textchars = bytearray({7,8,9,10,12,13,27} | set(range(0x20, 0x100)) - {0x7f})
    with open(filename, 'rb') as f:
        s = f.read(512)
    return bool(s.translate(None, textchars))


#Replace strings in the template
def profileReplaceStrings( src, dst, args) :
    if is_binary(src):
        shutil.copyfile(src, dst)
    else:
        with open( dst, "w") as fout, \
                open( src, "r") as fin:
            for line in fin:
                if args.modules == '_NO_' and '[LIST_OF_MODULES]' in line:
                    line = ''
                l = line.replace( '[LIST_OF_MODULES]', args.modules )
                l = l.replace( '[VERSION]', args.version )
                l = l.replace( '[SLOGAN]', args.slogan )
                l = l.replace( '[NAME_OF_APPLICATION]', args.name.upper() )
                l = l.replace( '[Name_of_Application]', args.name )
                l = l.replace( '(name_of_application)', args.name.lower() )
                fout.write( l )


#Generation of a template profile sources
def profileGenerateSources( args ) :

    #Set name of several directories
    app_dir = args.prefix
    app_resources_dir = os.path.join( app_dir, "resources" )
    kernel_root_dir = os.environ["KERNEL_ROOT_DIR"]
    bin_salome_dir = os.path.join( kernel_root_dir, "bin", "salome" )
    kernel_resources_dir = os.path.join( kernel_root_dir, "share", "salome", "resources", "kernel" )
    template_dir = os.path.join( kernel_resources_dir, "app-template" )

    #Check if the directory of the sources already exists and delete it
    if os.path.exists( app_dir ) :
        if not args.force :
            print("Directory %s already exists." %app_dir)
            print("Use option --force to overwrite it.")
            return
        else :
            shutil.rmtree( app_dir )

    #Copy template directory
    os.mkdir( app_dir )
    for root, dirs, files in os.walk( template_dir ) :
        dst_dir = root.replace( template_dir, app_dir ) 
        for d in dirs :
            os.mkdir( os.path.join( dst_dir, d ) )
        for f in files :
            profileReplaceStrings( os.path.join( root, f ), os.path.join( dst_dir, f ), args)

    #Complete source directory
    contextFiles = [ "salomeContext.py", "salomeContextUtils.py", "parseConfigFile.py" ]
    for f in contextFiles :
        shutil.copy( os.path.join( bin_salome_dir, f ), os.path.join( app_dir, "src" ) )

    #Search for python modules Image, ImageDraw and ImageFont
    try:
        import imp
        imp.find_module('Image')
        imp.find_module('ImageDraw')
        imp.find_module('ImageFont')
        found = True
    except ImportError:
        found = False

    #Generate logo and splash
    logo_destination = os.path.join( app_resources_dir, 'app_logo.png')
    splash_destination = os.path.join( app_resources_dir, 'splash.png')
    about_destination = os.path.join( app_resources_dir, 'about.png')
    if found :
        import ImageFont
        font = ImageFont.truetype( os.path.join( kernel_resources_dir, "Anita semi square.ttf" ) , 18 )

        #Generate and save logo
        app_logo = profileGenerateLogo( args.name, font )
        app_logo.save( logo_destination, "PNG" )

        #Generate and splash screen and about image
        if args.slogan :
            subtext = args.slogan
        else :
            subtext = "Powered by SALOME"
        im = profileGenerateSplash( kernel_resources_dir, args.name, args.version, subtext )
        im.save( splash_destination, "PNG" )
        im.save( about_destination, "PNG" )
    else :
        gui_resources_dir = os.path.join( os.environ["GUI_ROOT_DIR"], "share", "salome", "resources", "gui" )
        logo_name = os.path.join( gui_resources_dir, "icon_applogo.png" )
        if os.path.exists( logo_name ) :
            shutil.copy( logo_name, logo_destination )
        about_name = os.path.join( gui_resources_dir, "icon_about.png" )
        if os.path.exists( about_name ) :
            shutil.copy( about_name, about_destination )
            shutil.copy( about_name, splash_destination )

    #End of script
    print("Sources of %s were generated in %s." %( args.name, app_dir ))


# -----------------------------------------------------------------------------

if __name__ == '__main__':
    #Get optional and positional args
    args = profileQuickStartParser().parse_args()

    #Check name of the application
    if not args.name :
        raise RuntimeError( "A name must be given to the application. Please use option --name." )

    #Check if the prefix's parent is a directory
    if not os.path.isdir( os.path.dirname( args.prefix ) ) :
        raise RuntimeError( "%s is not a directory." % os.path.dirname( args.prefix ) )

    #Generate sources of the profile
    profileGenerateSources( args )
