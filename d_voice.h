// --------------------------------------------------------------------------
// This file is part of the KAGOUYAR firmware.
//
//    KAGOUYAR firmware is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    KAGOUYAR firmware is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with KAGOUYAR firmware. If not, see <http://www.gnu.org/licenses/>.
// --------------------------------------------------------------------------

// voice source : 0 = Kb, 1 = MIDI, 2 = CV gate
uint32_t g_new_gate = 0;

void add_voice(uint32_t my_GATE_source, int32_t my_pitch, float velocity) {
//void add_voice(uint32_t my_GATE_source, int32_t my_pitch) {
    uint32_t empty = 0;
    uint32_t used = 0;
    uint32_t i;
    int32_t voice_to_use_priority;
    //uint32_t reuse_voice = 0;
    g_new_gate = 1; // pour le signal de reset des AR

    for (i=nb_voice; i--;) { // on compte le nb de voie vide et utilisés
        if (allvoice[i].v_GATE == 0) empty++;
        else used++;
    }
    if (empty > 0) {
        voice_to_use_priority = 1; // on prend la premiere disponible
    }
    else {
        voice_to_use_priority = -nb_voice; // pas de vide, dc on prend une voie utilisé : la plus ancienne
    }

    for (i=nb_voice; i--;) { // on test si on as la meme note precedement joué
        if ( (allvoice[i].v_GATE_source == my_GATE_source) && (allvoice[i].v_pitch == my_pitch) ) {
            voice_to_use_priority = allvoice[i].v_priority;
     //       reuse_voice = 1;
        } // cette note a deja ete joué, on garde la voie
    }

    for (i=nb_voice; i--;) {
        if (allvoice[i].v_priority == voice_to_use_priority) { // celle que l'on assigne
            allvoice[i].v_GATE_source = my_GATE_source;
            allvoice[i].v_priority = -1;
            //if( reuse_voice == 0) allvoice[i].v_ADSR_out = 0.; // on reset l'envelope ssi on ne reutilise pas la voie
            allvoice[i].v_pitch = my_pitch;
            allvoice[i].v_GATE = 1;
            allvoice[i].v_TRIG = 1;
            allvoice[i].v_velocity = velocity;
        } else {
            if (voice_to_use_priority < 0) { // si la nouvelle voie etait deja actif avant (et dc avais une priorité negative
                if ( (allvoice[i].v_priority > voice_to_use_priority) && (allvoice[i].v_priority < 0) ) allvoice[i].v_priority -= 1;
                // on descend celles qui etaient plus recente qu'elle (parmis les voie utilisés)
            } else { // on assigne une voie inactive (mais pas forcement la N°1)
                if (allvoice[i].v_priority > voice_to_use_priority)  allvoice[i].v_priority -= 1;
                    // on utilise une voie "vide", on redescend seulement celles qui etaient plus recente qu'elle
                if (allvoice[i].v_priority < 0)  allvoice[i].v_priority -= 1;
                    // ainsi que celles qui etaient utilisés
            }
        }
    }
}

void remove_voice(uint32_t my_GATE_source, int32_t my_pitch) {
    int32_t num_voice = -1;
    int32_t priorite = 0;
    uint32_t nb_vide = 0;
    int32_t i;

    for (i=nb_voice; i--;) { // on cherche la voie a suprimer
        if ( (allvoice[i].v_GATE >= 1) && (allvoice[i].v_GATE_source == my_GATE_source) && ((allvoice[i].v_pitch == my_pitch) || (my_GATE_source == 2))) {
            // si on a un gate, provenant de la meme source, avec le meme pitch (sauf pour gate analogique, on ne test pas le pitch)
            num_voice = i; // on as trouvé la voie a suprimer
            priorite = allvoice[i].v_priority; // forcement negatif
        }
        if(allvoice[i].v_GATE == 0) nb_vide++; // on compte aussi le nb de voie vide pour se mettre a la fin
    }
    if(num_voice >= 0) { // ssi on as une voie a suprimer
        for (i=nb_voice; i--;) { // on reorganise les voies
            //myvoice = allvoice[i];
            if(i == num_voice) { // la voie a suprimer
                allvoice[i].v_priority = nb_vide + 1;
                allvoice[i].v_GATE = 0;
                allvoice[i].v_TRIG = 0;
            }
            else if (allvoice[i].v_priority < priorite) {
                // une voie existante plus ancienne, qui le devient un peu moins
                allvoice[i].v_priority++;
            }
        }
    }
}

inline uint32_t test_voice(int32_t pitch) {
    uint32_t on = 0;

    for (uint32_t i=nb_voice; i--;) {
        if ( (allvoice[i].v_pitch == pitch) && (allvoice[i].v_GATE_source == 0) && (allvoice[i].v_GATE >= 1) )
            on = 1;
   }
   return on;
}
