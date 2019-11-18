23/12/16 - Day 6: created ReadMe.txt file.
Here we write every happy things about this going-on project.
Enjoy this moment. You're living your real life. Alone, in a messing room, with music and the passion.

//Task for tomorrow:
- give Mover a dead
- put trees and more bushes
- create perlin noise tree
- create first monster

5/1/2017 - Day 19: there're still some bug on controling the mover
threw away the skeleton of mover and replaced by a single body
what should i do next:
-create a machine with sharp gears rotating - dangerous
-create a rocket shoter - pointing to mover every time mover is in the dangerous distance
-and fix the bug of controlling with the phenomena is: 
	when game start, mover can jump and move and shoot normally
	but when i press and hold,at once, jump button and shot button
	the mover keeps jumping for more than one jump.
	and after that moment, the mover is still keep jumping like that
	as I only press jump button.
	that is really a bug.
	-->all this mess is fixed by only one line of code in file Mover.cpp line 211 -> fucking funny =_=

8/1/2017 - Day 22: I find out a new way to delete every thing at one moment
as brick's destroyed, it should send a message to all of the objects hung on it
that means brick should hold all the pointer to the objects
after any thing is created, keep it!
--we need the grear killer must be stable. that means it 's the same at a position
--and to solve the problem of borning many at one position we will use the above solution


11/1/2017: - Day 25: Creating a mirror - Reflector for the lasers to be reflected
but there is a problem that is: I used all the digital letter from 0 to 9
so now I trying to use the letter 'abc' but It not work.
 and,of course I don't know why

 if the mirror work, the laser will be changed its direction which creates a very flexible scence

After Mirror what next are  a flock of bird maybe, a brigde, a pool of fishes, a dog

now get back to study for class ---===---

28/1/2017 - Day 42: Happy new year
-Every things are almost completed
-I have thunder clouds left
-after that, I will create a Boss
-then, rerange the map for more logic and game-likelihood
-finally, create Menu
-and find the way to release game
Continue..

30/1/2017 -  day 44: 5h30 am I cannot sleep
I have a crazy bug that make me crazy
it is: when the mover die. for one or two time. usually one time.
after that the program crash and inform that their is a empty pointer to mover->die()
that means the function die is called  by a empty pointer mover.
so i can understand that is because of the gamecontactListener makes a cast on ObjectData->pointer
and that pointer points to a empty mover. or that mover does not exist
or that mover is casted by another thing , if so there must be a huge mistake here, between other thing with mover
but the mover has function die. nothing else have it. there for this case is impossible ok.

----> 8h26 fix successfully the bug =)) very simple. just because of the MOVERID == 0
31/1/2017 - day 45: I realize some kind of bug by myself
that is: when a bridge mark (or some thing the same that) is initialized but we go back and initialize another mark bihind that 
then the two will be connect without acknowledge.
the solution is: the two bidge marks of difference bridges are away from each other with the distance equal to the number
 indices to be displayed in map.
 or the two farest marks must be smaller than that number indices


 2/2/2017 - day 47: Every things almost work well.
 I'm gonna create levels for the game.
 There will be 4 levels.
 The first one includes: monster tomato, gear killer, white cloud
 The second level includes: monster tomato, gear killer, white cloud,laser,
 Level 3: monster tomato, gear killer, white cloud,laser, pool
 Level 4: everythings, and dark cloud, power pole

 12/2/2017 - day 57: 4level game, everything is almost done
 now. it's time to adjust and get ready to post it some where 

23/2/2017 Day 68: So long, so so long It's taking me to fix the bug - on android. 
I have been stressed for several days. there is no way for me to finish this game on android except code them all a gain
and I'm doing it. it's seeming to be a right choice. at least at the moment. 