#ifndef CONTAINERS_H
#define CONTAINERS_H

#include "scripts.h"
#include "define.h"
#ifndef NAME
   #define NAME                    SCRIPT_CONTAINR
#endif
#include "../headers/command.h"

/* Defines and Macros */

/* States */
#define STATE_ACTIVE                    (0)
#define STATE_INACTIVE                  (1)
#define STATE_WOOD                      (0)
#define STATE_METAL                     (1)
#define STATE_FRIDGE                    (2)
#define STATE_OTHER                     (3)
#define STATE_NON_DESTROY               (2)
#define STATE_STANDARD_LOCK             (0)
#define STATE_ELECTRIC_LOCK             (1)

#define box_mstr(x) (message_str(SCRIPT_CONTAINR,x))
#ifdef custom_mstr
    #define my_mstr(x) (message_str(custom_mstr,x))
#else
    #define my_mstr box_mstr
#endif

/* Penalties for Lock difficulty based on whether or not you are using lockpicks. */
#ifndef Lock_Bonus
   #define Lock_Bonus                      (-20)
#endif
#ifndef Lockpick_Bonus
   #define Lockpick_Bonus                  (Lock_Bonus+20)
#endif
#ifndef Exd_Lockpick_Bonus
   #define Exd_Lockpick_Bonus              (Lock_Bonus+40)
#endif

/* Penalties for disarming the trap */
#ifndef Trap_Bonus
   #define Trap_Bonus                      (0)
#endif
#ifndef Trap_Set_Bonus
   #define Trap_Set_Bonus                  (Trap_Bonus-10)
#endif
/* Experience Points for Skills */
#ifndef Lockpick_Exp
   #define Lockpick_Exp                    EXP_LOCKPICK_NORMAL
#endif
#ifndef Traps_Exp
   #define Traps_Exp                       EXP_TRAPS_NORMAL
#endif

/* Penalties for forcing the door open using strength */
#ifndef Crowbar_Bonus
   #define Crowbar_Bonus                   (0)
#endif
#ifndef Crowbar_Strain
   #define Crowbar_Strain                  (2)
#endif

/* How many blasts can the door take before destorying */
#ifndef DOOR_STRENGTH
   #define DOOR_STRENGTH                   (1)
#endif

/* Max and Min damage for the trap */
#ifndef MIN_DAMAGE
   #define MIN_DAMAGE                      (10)
#endif
#ifndef MAX_DAMAGE
   #define MAX_DAMAGE                      (20)
#endif


/* Standard Script Procedures */
procedure start;
procedure use_p_proc;
#ifndef no_pickup
    procedure pickup_p_proc;
#endif
procedure look_at_p_proc;
procedure description_p_proc;
procedure use_skill_on_p_proc;
procedure use_obj_on_p_proc;
procedure damage_p_proc;
procedure map_enter_p_proc;
procedure map_update_p_proc;

/* Script Specific Procedure Calls */
procedure Look_Traps_And_Locks;
procedure Look_Traps;
procedure Look_Locks;
procedure Skill_Disarm_Traps;
procedure Disarm_Traps;
procedure Unlock_Lock;
procedure Lockpick_Lock;
procedure Lockpick_Door;
procedure Lock_Door;
procedure Set_Lock;
procedure Set_Lockpick_Lock;
procedure Set_Trap;
procedure Pry_Door;
procedure Damage_Critter;
procedure Super_Lockpick_Lock; //added by killap
procedure Super_Set_Lockpick_Lock; //added by killap

/*****************************************************************
   Local Variables which are saved. All Local Variables need to be
   prepended by LVAR_
*****************************************************************/
#define LVAR_Locked                     (0)
#define LVAR_Trapped                    (1)
#define LVAR_Found_Trap                 (2)
#define LVAR_Set_Door_Status            (3)
#define LVAR_Explosion_Attempts         (4)
#define LVAR_Gave_Locks_XP              (5)
#define LVAR_Gave_Traps_XP              (6)

/*******************************************************************
   Local variables which do not need to be saved between map changes.
*******************************************************************/
variable Locks_Roll;
variable Traps_Roll;

#define std_lockpick_inner_block \
   if (Tool == PID_LOCKPICKS) then begin \
      script_overrides; \
      if (local_var(LVAR_Locked) == STATE_ACTIVE) then begin \
         call Lockpick_Lock; \
      end else begin \
         call Set_Lockpick_Lock; \
      end \
   end \
   else if (Tool == PID_EXP_LOCKPICK_SET) then begin \
      script_overrides; \
      if (local_var(LVAR_Locked) == STATE_ACTIVE) then begin \
         call Super_Lockpick_Lock; \
      end else begin \
         call Super_Set_Lockpick_Lock; \
      end \
   end

#define std_lockpick_outer_block \
   if (LOCK_STATUS == STATE_STANDARD_LOCK) then begin \
      std_lockpick_inner_block \
   end

#define elec_lockpick_inner_block \
   if (Tool == PID_ELECTRONIC_LOCKPICKS) then begin \
      script_overrides; \
      if (local_var(LVAR_Locked) == STATE_ACTIVE) then begin \
         call Lockpick_Lock; \
      end else begin \
         call Set_Lockpick_Lock; \
      end \
   end \
   else if (Tool == PID_ELEC_LOCKPICK_MKII) then begin \
      script_overrides; \
      if (local_var(LVAR_Locked) == STATE_ACTIVE) then begin \
         call Super_Lockpick_Lock; \
      end else begin \
         call Super_Set_Lockpick_Lock; \
      end \
   end \

#define elec_lockpick_outer_block \
   else if (LOCK_STATUS == STATE_ELECTRIC_LOCK) then begin \
      elec_lockpick_inner_block \
   end


#define full_lockpick_block \
   std_lockpick_outer_block \
   elec_lockpick_outer_block

#ifndef no_pickup
    #ifndef custom_pickup_p_proc
        procedure pickup_p_proc begin
            call use_p_proc;
        end
    #endif
#endif

/**************************************************************************************
   Should the trap go off for any reason by critter influence, then this procedure will
   be called to deal damage to the critter, display a message stating how much damage
   was taken, and remove the trap.
**************************************************************************************/
#ifndef custom_Damage_Critter
procedure Damage_Critter begin
   variable Trap_Damage;

   Trap_Damage:=random(MIN_DAMAGE,MAX_DAMAGE);

   if (source_obj == dude_obj) then begin
       critter_dmg(dude_obj,Trap_Damage,(DMG_explosion BWOR DMG_BYPASS_ARMOR));
/* display_msg("You set off the trap and were hit for "+Trap_Damage+" points of damage.);                 */
       display_msg(my_mstr(166)+Trap_Damage+my_mstr(167));
   end

   else begin
       critter_dmg(source_obj,Trap_Damage,(DMG_explosion BWOR DMG_BYPASS_ARMOR));
/* display_msg(Critter_Name+" set off the trap was hit for "+Trap_Damage+" points of damage");        */
       display_msg(obj_name(source_obj)+my_mstr(168)+Trap_Damage+my_mstr(169));
   end

/* The trap is now disarmed and should never go off again.                           */
   set_local_var(LVAR_Trapped, STATE_INACTIVE);
end
#endif

/**********************************************************************************
   This is called when the player is using an object on the door. When the check is
   made to find out what is being used, obj_pid(obj_being_used_with) will need to
   be checked against a prototype.
**********************************************************************************/
#ifndef custom_use_obj_on_p_proc
procedure use_obj_on_p_proc begin
   variable Tool;

   Tool:=obj_pid(obj_being_used_with);

   full_lockpick_block

   else if (Tool == PID_CROWBAR) then begin
       script_overrides; //added by killap
       call Pry_Door;
   end

   else if ((Tool == PID_DYNAMITE) or (Tool == PID_PLASTIC_EXPLOSIVES)) then begin
       script_overrides; //added by killap
       call Set_Trap;
   end

end
#endif

#ifndef custom_start
procedure start begin end
#endif

/***************************************************************************
   This is cursory glance description that the player will receive should
   he just pass the Action Cursor over. Examines which give more information
   need to be in the description_p_proc procedure.
***************************************************************************/
#ifndef custom_look_at_p_proc
procedure look_at_p_proc begin
    script_overrides; 
    if (DOOR_STATUS == STATE_WOOD) then begin
        display_msg(my_mstr(100));
    end
    else if (DOOR_STATUS == STATE_FRIDGE) then begin
        display_msg(my_mstr(212));
    end
    else begin
        display_msg(my_mstr(101));
    end
end
#endif

// /***************************************************************************
//    This procedure will actively set the lock on the door and set all of
//    the coresponding varaibles for it.
// ***************************************************************************/
#ifndef custom_Lock_Door
procedure Lock_Door begin
   if (is_success(Locks_Roll)) then begin
       set_local_var(LVAR_Locked, STATE_ACTIVE);                // Door is unlocked
       obj_lock(self_obj);                                      // engine unlock door

       if (source_obj == dude_obj) then begin
           display_msg(my_mstr(189));
       end

       else begin
           display_msg(obj_name(source_obj)+my_mstr(191));
       end

       if (local_var(LVAR_Gave_Locks_XP) == 0) then begin
           set_local_var(LVAR_Gave_Locks_XP,1);
           give_xp(Lockpick_Exp);
       end
   end

   else begin
       if (source_obj == dude_obj) then begin
           display_msg(my_mstr(190));
       end

       else begin
           display_msg(obj_name(source_obj)+my_mstr(192));
       end
   end
end
#endif

/***************************************************************************
   This procedure will allow the source_obj to lock the door that he had
   just unlocked.
***************************************************************************/
#ifndef custom_Unlock_Lock
procedure Unlock_Lock begin
   Locks_Roll:=roll_vs_skill(source_obj,SKILL_LOCKPICK,Lock_Bonus);
   call Lockpick_Door;
end
#endif
#ifndef custom_Lockpick_Lock
procedure Lockpick_Lock begin
   Locks_Roll:=roll_vs_skill(source_obj,SKILL_LOCKPICK,Lockpick_Bonus);
   call Lockpick_Door;
end
#endif
#ifndef custom_Set_Lock
procedure Set_Lock begin
   Locks_Roll:=roll_vs_skill(source_obj,SKILL_LOCKPICK,Lock_Bonus);
   call Lock_Door;
end
#endif
#ifndef custom_Set_Lockpick_Lock
procedure Set_Lockpick_Lock begin
   Locks_Roll:=roll_vs_skill(source_obj,SKILL_LOCKPICK,Lockpick_Bonus);
   call Lock_Door;
end
#endif
#ifndef custom_Super_Lockpick_Lock
procedure Super_Lockpick_Lock begin
  Locks_Roll:=roll_vs_skill(source_obj,SKILL_LOCKPICK,Exd_Lockpick_Bonus);
  call Lockpick_Door();
end
#endif
#ifndef custom_Super_Set_Lockpick_Lock
procedure Super_Set_Lockpick_Lock begin
   Locks_Roll:=roll_vs_skill(source_obj,SKILL_LOCKPICK,Exd_Lockpick_Bonus);
   call Lock_Door;
end
#endif

/***************************************************************************
   This procedure will check against the roll_vs_lockpick to see if the
   source_object will unlock the door and display the appropriate messages
***************************************************************************/
#ifndef custom_Lockpick_Door
procedure Lockpick_Door begin
   if (is_success(Locks_Roll)) then begin
       set_local_var(LVAR_Locked, STATE_INACTIVE);              // Door is unlocked
       obj_unlock(self_obj);                                    // engine unlock door

       if (source_obj == dude_obj) then begin
           display_msg(my_mstr(170));
       end

       else begin
           display_msg(obj_name(source_obj)+my_mstr(173));
       end

       if (local_var(LVAR_Gave_Locks_XP) == 0) then begin
           set_local_var(LVAR_Gave_Locks_XP,1);
           give_xp(Lockpick_Exp);
       end
   end

   else if (is_critical(Locks_Roll)) then begin
       jam_lock(self_obj);

       if (source_obj == dude_obj) then begin
           display_msg(my_mstr(171));
       end

       else begin
           display_msg(obj_name(source_obj)+my_mstr(174));
       end
   end

   else begin
       if (source_obj == dude_obj) then begin
           display_msg(my_mstr(172));
       end

       else begin
           display_msg(obj_name(source_obj)+my_mstr(175));
       end
   end
end
#endif

/***************************************************************************
   This procedure will do a skill check vs traps to determine if the player
   or NPC finds the trap. If the trap is found, then it will check to see if
   the player or NPC set off the trap or disarms it.
***************************************************************************/
#ifndef custom_Disarm_Traps
procedure Disarm_Traps begin
    if (is_success(Traps_Roll)) then begin
        if (local_var(LVAR_Found_Trap) == 0) then begin
            set_local_var(LVAR_Found_Trap,1);
            if (source_obj == dude_obj) then begin
                display_msg(my_mstr(194));
            end
 
            else begin
                display_msg(my_mstr(199));
            end
 
            if (local_var(LVAR_Gave_Traps_XP) == 0) then begin
                set_local_var(LVAR_Gave_Traps_XP,1);
                give_xp(Traps_Exp);
            end
        end
 
        else begin
            set_local_var(LVAR_Trapped,STATE_INACTIVE);
            if (source_obj == dude_obj) then begin
                display_msg(my_mstr(196));
            end
 
            else begin
                display_msg(obj_name(source_obj)+my_mstr(201));
            end
        end
    end
 
    else if (is_critical(Traps_Roll)) then begin
        call Damage_Critter;
    end
 
    else begin
        if (local_var(LVAR_Found_Trap) == 0) then begin
            if (source_obj == dude_obj) then begin
                display_msg(my_mstr(195));
            end
 
            else begin
                display_msg(obj_name(source_obj)+my_mstr(200));
            end
        end
 
        else begin
            display_msg(obj_name(source_obj)+my_mstr(202));
        end
    end
end 
#endif

/*********************************************************************************
   This procedure will allow the player to set traps on doors behind him. The door
   will need to be closed, as all traps are set to go off if the door is openned.
*********************************************************************************/
#ifndef custom_Set_Trap
procedure Set_Trap begin
   variable Explosive;
   variable Removal_Counter;

/* Trap_Roll is a global variable to this script, defined at the beginning
   of the script. */

   Traps_Roll:=roll_vs_skill(source_obj,SKILL_TRAPS,Trap_Set_Bonus);

   Explosive:=obj_being_used_with;

   if (obj_is_open(self_obj)) then begin
       script_overrides;
       if (source_obj == dude_obj) then begin
           display_msg(my_mstr(206));
       end

       else begin
           display_msg(obj_name(source_obj)+my_mstr(207));
       end
   end

   else begin
       script_overrides;
       Removal_Counter:=rm_mult_objs_from_inven(source_obj,Explosive,1);

       if (is_success(Traps_Roll)) then begin
           set_local_var(LVAR_Trapped,STATE_ACTIVE);
           if (source_obj == dude_obj) then begin
               display_msg(my_mstr(208));
           end

           else begin
               display_msg(obj_name(source_obj)+my_mstr(210));
           end
       end

       else if (is_critical(Traps_Roll)) then begin
           call Damage_Critter;
       end

       else begin
           if (source_obj == dude_obj) then begin
               display_msg(my_mstr(209));
           end

           else begin
               display_msg(obj_name(source_obj)+my_mstr(211));
           end
       end
   end
end
#endif

/*********************************************************************************
   This procedure will do a standard Traps roll to see if the player can
   find and remove traps from the door.
*********************************************************************************/
#ifndef custom_Skill_Disarm_Traps
procedure Skill_Disarm_Traps begin
   /* Trap_Roll is a global variable to this script, defined at the beginning
      of the script. */   
   Traps_Roll:=roll_vs_skill(source_obj,SKILL_TRAPS,Trap_Bonus);
   call Disarm_Traps;
end
#endif

/*********************************************************************************
   This is the set of conditions for when you look at a locked door, based on your
   lockpick skill and perception to notice the lock.
*********************************************************************************/
#ifndef custom_Look_Locks
procedure Look_Locks begin
   variable Perception_Check;
   variable Locks_Check;

   Perception_Check:=do_check(dude_obj,STAT_pe,0);
   Locks_Check:=roll_vs_skill(dude_obj,SKILL_LOCKPICK,0);

   if (is_success(Perception_Check)) then begin
       if (is_critical(Perception_Check)) then begin
           if (is_success(Locks_Check)) then begin
               if (is_critical(Locks_Check)) then begin
                   display_msg(my_mstr(114));
               end                                              // Critical Success (Locks)

               else begin
                   display_msg(my_mstr(115));
               end                                              // Regular Success (Locks)
           end

           else if (is_critical(Locks_Check)) then begin
               display_msg(my_mstr(116));
           end                                                  // Critical Failure (Locks)

           else begin
               display_msg(my_mstr(117));
           end                                                  // Regular Failure (Locks)
       end                                                      // Critical Success (Stat_Pe)

       else begin
           if (is_success(Locks_Check)) then begin
               if (is_critical(Locks_Check)) then begin
                   display_msg(my_mstr(130));
               end                                              // Critical Success (Locks)

               else begin
                   display_msg(my_mstr(131));
               end                                              // Regular Success (Locks)
           end

           else if (is_critical(Locks_Check)) then begin
               display_msg(my_mstr(132));
           end                                                  // Critical Failure (Locks)

           else begin
               display_msg(my_mstr(133));
           end                                                  // Regular Failure (Locks)
       end                                                      // Regular Success (Stat_pe)
   end

   else if (is_critical(Perception_Check)) then begin
       if (is_success(Locks_Check)) then begin
           if (is_critical(Locks_Check)) then begin
               display_msg(my_mstr(146));
           end                                                  // Critical Success (Locks)

           else begin
               display_msg(my_mstr(147));
           end                                                  // Regular Success (Locks)
       end

       else if (is_critical(Locks_Check)) then begin
           display_msg(my_mstr(148));
       end                                                      // Critical Failure (Locks)

       else begin
           display_msg(my_mstr(149));
       end                                                      // Regular Failure (Locks)
   end                                                          // Critical Failure (Stat_pe)

   else begin
       if (is_success(Locks_Check)) then begin
           if (is_critical(Locks_Check)) then begin
               display_msg(my_mstr(162));
           end                                                  // Critical Success (Locks)

           else begin
               display_msg(my_mstr(163));
           end                                                  // Regular Success (Locks)
       end

       else if (is_critical(Locks_Check)) then begin
           display_msg(my_mstr(164));
       end                                                      // Critical Failure (Locks)

       else begin
           display_msg(my_mstr(165));
       end                                                      // Regular Failure (Locks)
   end                                                          // Regular Failure (Stat_Pe)
end
#endif

/**********************************************************************************
   This is the set of conditions for when you look at a trapped door, based on your
   traps skill and perception to notice the trap.
**********************************************************************************/
#ifndef custom_Look_Traps
procedure Look_Traps begin
   variable Perception_Check;
   variable Traps_Check;

   Perception_Check:=do_check(dude_obj,STAT_pe,0);
   Traps_Check:=roll_vs_skill(dude_obj,SKILL_TRAPS,0);

   if (is_success(Perception_Check)) then begin
       if (is_critical(Perception_Check)) then begin
           if (is_success(Traps_Check)) then begin
               if (is_critical(Traps_Check)) then begin
                   display_msg(my_mstr(104));
               end                                              // Critical Success (traps)

               else begin
                   display_msg(my_mstr(108));
               end                                              // Regular Success (Traps)
           end

           else if (is_critical(Traps_Check)) then begin
               display_msg(my_mstr(112));
           end                                                  // Critical Failure (Traps)

           else begin
               display_msg(my_mstr(116));
           end                                                  // Regular Failure (Traps)
       end                                                      // Critical Success (Stat_Pe)

       else begin
           if (is_success(Traps_Check)) then begin
               if (is_critical(Traps_Check)) then begin
                   display_msg(my_mstr(120));
               end                                              // Critical Success (traps)

               else begin
                   display_msg(my_mstr(124));
               end                                              // Regular Success (Traps)
           end

           else if (is_critical(Traps_Check)) then begin
               display_msg(my_mstr(128));
           end                                                  // Critical Failure (Traps)

           else begin
               display_msg(my_mstr(132));
           end                                                  // Regular Failure (Traps)
       end                                                      // Regular Success (Stat_pe)
   end

   else if (is_critical(Perception_Check)) then begin
       if (is_success(Traps_Check)) then begin
           if (is_critical(Traps_Check)) then begin
               display_msg(my_mstr(136));
           end                                                  // Critical Success (traps)

           else begin
               display_msg(my_mstr(140));
           end                                                  // Regular Success (Traps)
       end

       else if (is_critical(Traps_Check)) then begin
           display_msg(my_mstr(144));
       end                                                      // Critical Failure (Traps)

       else begin
           display_msg(my_mstr(148));
       end                                                      // Regular Failure (Traps)
   end                                                          // Critical Failure (Stat_pe)

   else begin
       if (is_success(Traps_Check)) then begin
           if (is_critical(Traps_Check)) then begin
               display_msg(my_mstr(152));
           end                                                  // Critical Success (traps)

           else begin
               display_msg(my_mstr(156));
           end                                                  // Regular Success (Traps)
       end

       else if (is_critical(Traps_Check)) then begin
           display_msg(my_mstr(160));
       end                                                      // Critical Failure (Traps)

       else begin
           display_msg(my_mstr(164));
       end                                                      // Regular Failure (Traps)
   end                                                          // Regular Failure (Stat_Pe)

end
#endif

/*************************************************************************************
   This is the set of conditions for when you look at a locked and trapped door, based
   on your lockpick and traps skills and perception to notice things.
*************************************************************************************/
#ifndef custom_Look_Traps_And_Locks
procedure Look_Traps_And_Locks begin
   variable Perception_Check;
   variable Traps_Check;
   variable Locks_Check;

   Perception_Check:=do_check(dude_obj,STAT_pe,0);
   Traps_Check:=roll_vs_skill(dude_obj,SKILL_TRAPS,0);
   Locks_Check:=roll_vs_skill(dude_obj,SKILL_LOCKPICK,0);

   if (is_success(Perception_Check)) then begin

/*************************************************************************************************/

      /* Critical Success of a Perception Check  (Start)*/

       if (is_critical(Perception_Check)) then begin

           if (is_success(Traps_Check)) then begin
               set_local_var(LVAR_Found_Trap,1);                // player has found the trap

      /* Critical Success of a Traps Check  (Start)*/

               if (is_critical(Traps_Check)) then begin

                   if (is_success(Locks_Check)) then begin
                       if (is_critical(Locks_Check)) then begin
                           display_msg(my_mstr(102));
                       end                                      // critical success (Locks)

                       else begin
                           display_msg(my_mstr(103));
                       end                                      // regular success (Locks)
                   end

                   else if (is_critical(Locks_Check)) then begin
                       display_msg(my_mstr(104));
                   end                                          // critical Failure (Locks)

                   else begin
                       display_msg(my_mstr(105));
                   end                                          // end of regular failure (Locks)
               end                                              // critical success (Traps)

      /* Success of a Traps Check  (Start)*/

               else begin
                   if (is_success(Locks_Check)) then begin
                       if (is_critical(Locks_Check)) then begin
                           display_msg(my_mstr(106));
                       end                                      // critical success (Locks)

                       else begin
                           display_msg(my_mstr(107));
                       end                                      // regular success (Locks)
                   end

                   else if (is_critical(Locks_Check)) then begin
                       display_msg(my_mstr(108));
                   end                                          // critical Failure (Locks)

                   else begin
                       display_msg(my_mstr(109));
                   end                                          // end of regular failure (Locks)
               end                                              // regular success (Traps)
           end

      /* Critical Failure of a Traps Check  (Start)*/

           else if (is_critical(Traps_Check)) then begin
               if (is_success(Locks_Check)) then begin
                   if (is_critical(Locks_Check)) then begin
                       display_msg(my_mstr(110));
                   end                                          // critical success (Locks)

                   else begin
                       display_msg(my_mstr(111));
                   end                                          // regular success (Locks)
               end

               else if (is_critical(Locks_Check)) then begin
                   display_msg(my_mstr(112));
               end                                              // critical Failure (Locks)

               else begin
                   display_msg(my_mstr(113));
               end                                              // end of regular failure (Locks)
          end                                                   // critical Failure (Traps)

      /* Failure of a Traps Check  (Start)*/

           else begin
               if (is_success(Locks_Check)) then begin
                   if (is_critical(Locks_Check)) then begin
                       display_msg(my_mstr(114));
                   end                                          // critical success (Locks)

                   else begin
                       display_msg(my_mstr(115));
                   end                                          // regular success (Locks)
               end

               else if (is_critical(Locks_Check)) then begin
                   display_msg(my_mstr(116));
               end                                              // critical Failure (Locks)

               else begin
                   display_msg(my_mstr(117));
               end                                              // end of regular failure (Locks)
           end                                                  // regular failure (Traps)
       end                                                      // critical success (Stat_Pe)

      /* Critical Success of a Perception Check  (Finish)*/

/*************************************************************************************************/

      /* Success of a Perception Check  (Start)*/

       else begin
           if (is_success(Traps_Check)) then begin
               set_local_var(LVAR_Found_Trap,1);                // player has found the trap

      /* Critical Success of a Traps Check  (Start)*/

               if (is_critical(Traps_Check)) then begin

                   if (is_success(Locks_Check)) then begin
                       if (is_critical(Locks_Check)) then begin
                           display_msg(my_mstr(118));
                       end                                      // critical success (Locks)

                       else begin
                           display_msg(my_mstr(119));
                       end                                      // regular success (Locks)
                   end

                   else if (is_critical(Locks_Check)) then begin
                       display_msg(my_mstr(120));
                   end                                          // critical Failure (Locks)

                   else begin
                       display_msg(my_mstr(121));
                   end                                          // end of regular failure (Locks)
               end                                              // critical success (Traps)

      /* Success of a Traps Check  (Start)*/

               else begin
                   if (is_success(Locks_Check)) then begin
                       if (is_critical(Locks_Check)) then begin
                           display_msg(my_mstr(122));
                       end                                      // critical success (Locks)

                       else begin
                           display_msg(my_mstr(123));
                       end                                      // regular success (Locks)
                   end

                   else if (is_critical(Locks_Check)) then begin
                       display_msg(my_mstr(124));
                   end                                          // critical Failure (Locks)

                   else begin
                       display_msg(my_mstr(125));
                   end                                          // end of regular failure (Locks)
               end                                              // regular success (Traps)
           end

      /* Critical Failure of a Traps Check  (Start)*/

           else if (is_critical(Traps_Check)) then begin
               if (is_success(Locks_Check)) then begin
                   if (is_critical(Locks_Check)) then begin
                       display_msg(my_mstr(126));
                   end                                          // critical success (Locks)

                   else begin
                       display_msg(my_mstr(127));
                   end                                          // regular success (Locks)
               end

               else if (is_critical(Locks_Check)) then begin
                   display_msg(my_mstr(128));
               end                                              // critical Failure (Locks)

               else begin
                   display_msg(my_mstr(129));
               end                                              // end of regular failure (Locks)
           end                                                  // critical Failure (Traps)

      /* Failure of a Traps Check  (Start)*/

           else begin
               if (is_success(Locks_Check)) then begin
                   if (is_critical(Locks_Check)) then begin
                       display_msg(my_mstr(130));
                   end                                          // critical success (Locks)

                   else begin
                       display_msg(my_mstr(131));
                   end                                          // regular success (Locks)
               end

               else if (is_critical(Locks_Check)) then begin
                   display_msg(my_mstr(132));
               end                                              // critical Failure (Locks)

               else begin
                   display_msg(my_mstr(133));
               end                                              // end of regular failure (Locks)
           end                                                  // regular failure (Traps)
       end                                                      // regular success (Stat_pe)
   end

      /* Success of a Perception Check  (Finish)*/

/*************************************************************************************************/

      /* Critical Failure of a Perception Check  (Start)*/

   else if (is_critical(Perception_Check)) then begin
       if (is_success(Traps_Check)) then begin
           set_local_var(LVAR_Found_Trap,1);                    // player has found the trap

      /* Critical Success of a Traps Check  (Start)*/

           if (is_critical(Traps_Check)) then begin

               if (is_success(Locks_Check)) then begin
                   if (is_critical(Locks_Check)) then begin
                       display_msg(my_mstr(134));
                   end                                          // critical success (Locks)

                   else begin
                       display_msg(my_mstr(135));
                   end                                          // regular success (Locks)
               end

               else if (is_critical(Locks_Check)) then begin
                   display_msg(my_mstr(136));
               end                                              // critical Failure (Locks)

               else begin
                   display_msg(my_mstr(137));
               end                                              // end of regular failure (Locks)
           end                                                  // critical success (Traps)

      /* Success of a Traps Check  (Start)*/

           else begin
               if (is_success(Locks_Check)) then begin
                   if (is_critical(Locks_Check)) then begin
                       display_msg(my_mstr(138));
                   end                                          // critical success (Locks)

                   else begin
                       display_msg(my_mstr(139));
                   end                                          // regular success (Locks)
               end

               else if (is_critical(Locks_Check)) then begin
                   display_msg(my_mstr(140));
               end                                              // critical Failure (Locks)

               else begin
                   display_msg(my_mstr(141));
               end                                              // end of regular failure (Locks)
           end                                                  // regular success (Traps)
       end

      /* Critical Failure of a Traps Check  (Start)*/

       else if (is_critical(Traps_Check)) then begin
           if (is_success(Locks_Check)) then begin
               if (is_critical(Locks_Check)) then begin
                   display_msg(my_mstr(142));
               end                                              // critical success (Locks)

               else begin
                   display_msg(my_mstr(143));
               end                                              // regular success (Locks)
           end

           else if (is_critical(Locks_Check)) then begin
               display_msg(my_mstr(144));
           end                                                  // critical Failure (Locks)

           else begin
               display_msg(my_mstr(145));
           end                                                  // end of regular failure (Locks)
       end                                                      // critical Failure (Traps)

      /* Failure of a Traps Check  (Start)*/

       else begin
           if (is_success(Locks_Check)) then begin
               if (is_critical(Locks_Check)) then begin
                   display_msg(my_mstr(146));
               end                                              // critical success (Locks)

               else begin
                   display_msg(my_mstr(147));
               end                                              // regular success (Locks)
           end

           else if (is_critical(Locks_Check)) then begin
               display_msg(my_mstr(148));
           end                                                  // critical Failure (Locks)

           else begin
               display_msg(my_mstr(149));
           end                                                  // end of regular failure (Locks)
       end                                                      // regular failure (Traps)
   end                                                          // Critical Failure (Stat_pe)

      /* Critical Failure of a Perception Check  (Finish)*/

/*************************************************************************************************/

      /* Failure of a Perception Check  (Start)*/

   else begin
       if (is_success(Traps_Check)) then begin
           set_local_var(LVAR_Found_Trap,1);                    // player has found the trap

      /* Critical Success of a Traps Check  (Start)*/

           if (is_critical(Traps_Check)) then begin

               if (is_success(Locks_Check)) then begin
                   if (is_critical(Locks_Check)) then begin
                       display_msg(my_mstr(150));
                   end                                          // critical success (Locks)

                   else begin
                       display_msg(my_mstr(151));
                   end                                          // regular success (Locks)
               end

               else if (is_critical(Locks_Check)) then begin
                   display_msg(my_mstr(152));
               end                                              // critical Failure (Locks)

               else begin
                   display_msg(my_mstr(153));
               end                                              // end of regular failure (Locks)
           end                                                  // critical success (Traps)

      /* Success of a Traps Check  (Start)*/

           else begin
               if (is_success(Locks_Check)) then begin
                   if (is_critical(Locks_Check)) then begin
                       display_msg(my_mstr(154));
                   end                                          // critical success (Locks)

                   else begin
                       display_msg(my_mstr(155));
                   end                                          // regular success (Locks)
               end

               else if (is_critical(Locks_Check)) then begin
                   display_msg(my_mstr(156));
               end                                              // critical Failure (Locks)

               else begin
                   display_msg(my_mstr(157));
               end                                              // end of regular failure (Locks)
           end                                                  // regular success (Traps)
       end

      /* Critical Failure of a Traps Check  (Start)*/

       else if (is_critical(Traps_Check)) then begin
           if (is_success(Locks_Check)) then begin
               if (is_critical(Locks_Check)) then begin
                   display_msg(my_mstr(158));
               end                                              // critical success (Locks)

               else begin
                   display_msg(my_mstr(159));
               end                                              // regular success (Locks)
           end

           else if (is_critical(Locks_Check)) then begin
               display_msg(my_mstr(160));
           end                                                  // critical Failure (Locks)

           else begin
               display_msg(my_mstr(161));
           end                                                  // end of regular failure (Locks)
       end                                                      // critical Failure (Traps)

      /* Failure of a Traps Check  (Start)*/

       else begin
           if (is_success(Locks_Check)) then begin
               if (is_critical(Locks_Check)) then begin
                   display_msg(my_mstr(162));
               end                                              // critical success (Locks)

               else begin
                   display_msg(my_mstr(163));
               end                                              // regular success (Locks)
           end

           else if (is_critical(Locks_Check)) then begin
               display_msg(my_mstr(164));
           end                                                  // critical Failure (Locks)

           else begin
               display_msg(my_mstr(165));
           end                                                  // end of regular failure (Locks)
       end                                                      // regular failure (Traps)
   end                                                          // Regular Failure (Stat_pe)

      /* Failure of a Perception Check  (Finish)*/

end
#endif

/******************************************************************************************
   Should the door sustain damage from anything (ie, dynamite, plastic explosives, rockets,
   or any other forms of damage), this procedure will be called to destroy the door and
   free up the space.
******************************************************************************************/
#ifndef custom_damage_p_proc
procedure damage_p_proc begin
  variable Trap_Damage;
   /************ Wood Door ****************/
 
  if (weapon_dmg_type(target_obj) == DMG_explosion) then begin
    if (DOOR_STATUS == STATE_WOOD) then begin
        if (local_var(LVAR_Trapped) == STATE_ACTIVE) then begin
            set_local_var(LVAR_Trapped, STATE_INACTIVE);
            set_local_var(LVAR_Locked, STATE_INACTIVE);
            Trap_Damage:=random(MIN_DAMAGE,MAX_DAMAGE);
            explosion(self_tile, self_elevation, Trap_Damage);
            destroy_object(self_obj);
        end
 
        else begin
            set_local_var(LVAR_Trapped, STATE_INACTIVE);
            set_local_var(LVAR_Locked, STATE_INACTIVE);
            destroy_object(self_obj);
        end
    end
 
   /************ Metal Door ****************/
 
    else if ((DOOR_STATUS == STATE_METAL) or (DOOR_STATUS == STATE_FRIDGE)) then begin
        set_local_var(LVAR_Explosion_Attempts,local_var(LVAR_Explosion_Attempts)+1);
        if (local_var(LVAR_Trapped) == STATE_ACTIVE) then begin
            set_local_var(LVAR_Locked, STATE_INACTIVE);
            Trap_Damage:=random(MIN_DAMAGE,MAX_DAMAGE);
            explosion(self_tile, self_elevation, Trap_Damage);
        end
 
        else if (local_var(LVAR_Explosion_Attempts) > DOOR_STRENGTH) then begin
            set_local_var(LVAR_Trapped, STATE_INACTIVE);
            set_local_var(LVAR_Locked,STATE_INACTIVE);
            destroy_object(self_obj);
        end
    end
  end
    else begin
        display_msg(my_mstr(193));
    end
 end
#endif


/***************************************************************************************
   Whenever the map is first entered, this procedure will be called. The main purpose of
   this procedure is to lock the door from the outset, rather than having to worry about
   it through Mapper. After the door has been unlocked, it will remain as such, unless
   the player locks it once more.
***************************************************************************************/
#ifndef custom_map_enter_p_proc
procedure map_enter_p_proc begin
/* Set up the door state when the player first enters the map */
   if (local_var(LVAR_Set_Door_Status) == 0) then begin
       set_local_var(LVAR_Set_Door_Status,1);
       set_local_var(LVAR_Locked,LOCKED_STATUS);
       set_local_var(LVAR_Trapped,TRAPPED_STATUS);
   end
   if (local_var(LVAR_Locked) == STATE_ACTIVE) then begin
       obj_lock(self_obj);
   end
   else begin
       obj_unlock(self_obj);
   end
end
#endif
/**************************************************************************************
   This procedure gets called roughly every 30 seconds of real time. It is used to make
   sure that the door does not lock on it's own and that the player will be able to get
   through once he has unlocked the door. Once more, this is so that locking and
   unlocking do not need to be done in Mapper.
**************************************************************************************/
#ifndef custom_map_update_p_proc
procedure map_update_p_proc begin
   if (local_var(LVAR_Locked) == STATE_ACTIVE) then begin
       obj_lock(self_obj);
   end

   else begin
       obj_unlock(self_obj);
   end
end
#endif

/**********************************************************************************
   Should the player try using a skill on the door, here are where the skill rolls
   will be made. The ones which need to be in here are: Lockpick and Traps. Science
   and Repair can be added to this list to give more information about the door.
**********************************************************************************/
#ifndef custom_use_skill_on_p_proc
procedure use_skill_on_p_proc begin
   variable Skill_Used;

   Skill_Used:=action_being_used;

   if (Skill_Used == SKILL_LOCKPICK) then begin
       script_overrides; //added by killap
       if (local_var(LVAR_Locked) == STATE_ACTIVE) then begin
           call Unlock_Lock;
       end
       else begin
           call Set_Lock;
       end
   end                                                          // End of Skill_Lockpick

   else if (Skill_Used == SKILL_TRAPS) then begin
       script_overrides; //added by killap
       if (local_var(LVAR_Trapped) == STATE_ACTIVE) then begin
           call Skill_Disarm_Traps;
       end
       else begin
           if (source_obj == dude_obj) then begin
               display_msg(my_mstr(195));
           end

           else begin
               display_msg(obj_name(source_obj)+my_mstr(200));
           end
       end
   end                                                          // End of Skill_Traps
end
#endif

/**********************************************************************************
   Should the player examine the door closely, then he should be allowed a lockpick
   roll, a traps roll and a perception roll. Depending on which rolls are made will
   determine how much information about the door will be given.
**********************************************************************************/
#ifndef custom_description_p_proc
procedure description_p_proc begin
    script_overrides;
    if ((local_var(LVAR_Locked) == STATE_ACTIVE) and (local_var(LVAR_Trapped) == STATE_ACTIVE)) then begin
        call Look_Traps_And_Locks;
    end
    else if (local_var(LVAR_Trapped) == STATE_ACTIVE) then begin
        call Look_Traps;
    end
    else if (local_var(LVAR_Locked) == STATE_ACTIVE) then begin
        call Look_Locks;
    end
    else if (DOOR_STATUS == STATE_WOOD) then begin
        display_msg(my_mstr(100));
    end
    else if (DOOR_STATUS == STATE_FRIDGE) then begin
        display_msg(my_mstr(212));
    end
    else begin
        display_msg(my_mstr(101));
    end
end
#endif

/********************************************************************
   Any time that a critter tries to use this door will call this
   procedure. it will check to see if the door is trapped and locked.
********************************************************************/
#ifndef custom_use_p_proc
procedure use_p_proc begin
/* Trap_Roll is a global variable to this script, defined at the beginning
   of the script. */

   Traps_Roll:=roll_vs_skill(source_obj,SKILL_TRAPS,Trap_Bonus);

   if (local_var(LVAR_Trapped) == STATE_ACTIVE) then begin
       if (is_success(Traps_Roll)) then begin
           script_overrides;
           set_local_var(LVAR_Found_Trap,1);
           reg_anim_clear(source_obj);

           if (source_obj == dude_obj) then begin
               display_msg(my_mstr(204));
           end
           else begin
               display_msg(my_mstr(205));
           end
       end

       else begin
           if (obj_is_locked(self_obj)) then begin
               script_overrides;
               display_msg(my_mstr(203));
               call Damage_Critter;
           end
           else begin
               call Damage_Critter;
           end
       end
   end

   else if (obj_is_locked(self_obj)) then begin
       script_overrides;
       display_msg(my_mstr(203));
   end
end
#endif

#endif // CONTAINERS2_H
