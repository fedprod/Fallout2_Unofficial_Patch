#ifndef door_mstr
   #define door_mstr(x)                    message_str(SCRIPT_DOOR, x)
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
       display_msg(door_mstr(166)+Trap_Damage+door_mstr(167));
   end

   else begin
       critter_dmg(source_obj,Trap_Damage,(DMG_explosion BWOR DMG_BYPASS_ARMOR));
/* display_msg(Critter_Name+" set off the trap was hit for "+Trap_Damage+" points of damage");        */
       display_msg(obj_name(source_obj)+door_mstr(168)+Trap_Damage+door_mstr(169));
   end

/* The trap is now disarmed and should never go off again.                           */
   set_local_var(LVAR_Trapped, STATE_INACTIVE);
end
#endif

/***************************************************************************
   This procedure is used should the player try to pry the door open using a
   crowbar or some similar instrument.
***************************************************************************/
procedure Pry_Door begin
   variable Stat_Roll;

   Stat_Roll:=do_check(source_obj,STAT_st,Crowbar_Bonus);

   if (is_success(Stat_Roll)) then begin
       set_local_var(LVAR_Locked, STATE_INACTIVE);
       obj_unlock(self_obj);

       if (source_obj == dude_obj) then begin
           display_msg(mstr(176));
       end

       else begin
           display_msg(mstr(181));
       end
   end

   else if (is_critical(Stat_Roll)) then begin
       critter_dmg(source_obj,Crowbar_Strain,(DMG_normal_dam BWOR DMG_BYPASS_ARMOR));

       if (source_obj == dude_obj) then begin
           if (Crowbar_Strain == 1) then begin
               display_msg(mstr(177));
           end
           else begin
               display_msg(mstr(178)+Crowbar_Strain+mstr(179));
           end
       end

       else begin
           if (is_male(source_obj)) then begin
               if (Crowbar_Strain == 1) then begin
                   display_msg(mstr(182));
               end
               else begin
                   display_msg(mstr(183)+Crowbar_Strain+mstr(184));
               end
           end

           else begin
               if (Crowbar_Strain == 1) then begin
                   display_msg(mstr(186));
               end
               else begin
                   display_msg(mstr(187)+Crowbar_Strain+mstr(188));
               end
           end
       end
   end

   else begin
       if (source_obj == dude_obj) then begin
           display_msg(mstr(180));
       end

       else begin
           display_msg(mstr(185));
       end
   end
end


/**********************************************************************************
   This is called when the player is using an object on the door. When the check is
   made to find out what is being used, obj_pid(obj_being_used_with) will need to
   be checked against a prototype.
**********************************************************************************/

procedure use_obj_on_p_proc begin
   variable Tool;

   Tool:=obj_pid(obj_being_used_with);

   if (LOCK_STATUS == STATE_STANDARD_LOCK) then begin
       if (Tool == PID_LOCKPICKS) then begin
           script_overrides; //added by killap
           if (local_var(LVAR_Locked) == STATE_ACTIVE) then begin
               call Lockpick_Lock;
           end
           else begin
               call Set_Lockpick_Lock;
           end
       end
       else if (Tool == PID_EXP_LOCKPICK_SET) then begin
           script_overrides; //added by killap
           if (local_var(LVAR_Locked) == STATE_ACTIVE) then begin
               call Super_Lockpick_Lock;
           end
           else begin
               call Super_Set_Lockpick_Lock;
           end
       end
   end

   else if (LOCK_STATUS == STATE_ELECTRIC_LOCK) then begin
       if (Tool == PID_ELECTRONIC_LOCKPICKS) then begin
           script_overrides; //added by killap
           if (local_var(LVAR_Locked) == STATE_ACTIVE) then begin
               call Lockpick_Lock;
           end
           else begin
               call Set_Lockpick_Lock;
           end
       end
       else if (Tool == PID_ELEC_LOCKPICK_MKII) then begin
           script_overrides; //added by killap
           if (local_var(LVAR_Locked) == STATE_ACTIVE) then begin
               call Super_Lockpick_Lock;
           end
           else begin
               call Super_Set_Lockpick_Lock;
           end
       end
   end

   else if (Tool == PID_CROWBAR) then begin
       script_overrides; //added by killap
       call Pry_Door;
   end

   else if ((Tool == PID_DYNAMITE) or (Tool == PID_PLASTIC_EXPLOSIVES)) then begin
       script_overrides; //added by killap
       call Set_Trap;
   end

end
