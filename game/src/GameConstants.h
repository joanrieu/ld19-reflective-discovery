#ifndef GAMECONSTANTS_H_
#define GAMECONSTANTS_H_

#define GC_TEXT_NAME "Discovery"
#define GC_TEXT_LEVELCHOICE "Choose level:"
#define GC_TEXT_LEVELERROR "Error encountered while loading level."
#define GC_TEXT_CONGRATULATIONS(id) (wxString("Congratulations ! You won the level ") << id << " !")
#define GC_TEXT_NEXTLEVEL(id) (wxString("You will now enter level ") << id << "...")
#define GC_TEXT_LEVELQUITCONFIRM "Are you sure you want to go back to the title screen ?"
#define GC_TEXT_QUITCONFIRM "Are you sure you want to quit ?"

#define GC_FRAME_WIDTH 800
#define GC_FRAME_HEIGHT 600

#define GC_LEVEL_FOLDER "../lvl/"
#define GC_LEVEL_FILE(id) (wxString(GC_LEVEL_FOLDER) << id << ".dat")
#define GC_LEVEL_BLOCK_SIZE 50
#define GC_LEVEL_WIDTH (GC_FRAME_WIDTH / GC_LEVEL_BLOCK_SIZE)
#define GC_LEVEL_HEIGHT (GC_FRAME_HEIGHT / GC_LEVEL_BLOCK_SIZE)

#define GC_ART_FOLDER "../art/"
#define GC_ART_PNG(file) file, wxBITMAP_TYPE_PNG
#define GC_ART_AUTO_PNG(col) GC_ART_PNG(wxString(GC_ART_FOLDER) << col << ".png")
#define GC_ART_TITLE GC_ART_AUTO_PNG("title")
#define GC_ART_BACKGROUND GC_ART_AUTO_PNG("background")
#define GC_ART_LASER GC_ART_AUTO_PNG("laser")
#define GC_ART_LASERS "lasers/"
#define GC_ART_LASERS_LIGHT "lasers_light/"
#define GC_ART_REFLECTORS "reflectors/"
#define GC_ART_BLOCKS "blocks/"
#define GC_ART_BLOCKS_LIGHT "blocks_light/"
#define GC_ART_MUSIC (wxString(GC_ART_FOLDER) << "background.wav")

#define GC_LOG(msg) wxMessageOutputStderr().Output(wxString() << __FILE__ ":" << __LINE__ << ": " << msg)

#endif
