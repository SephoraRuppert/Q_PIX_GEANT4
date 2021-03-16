import ROOT
import csv
import numpy as np
import matplotlib
matplotlib.use('Agg') # Must be before importing matplotlib.pyplot or pylab!
from mpl_toolkits import mplot3d
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt

if __name__ == "__main__":

    # Read in the root file
   inFileName = "/n/home04/sruppert/Q_PIX_GEANT4/gxe/output/electron.root"
   inFile = ROOT.TFile.Open(inFileName, "READ")

   # Read the trees in the root file
   tree1 = inFile.Get("event_tree")

   # Loop over events in the tree
   for event in range(0, tree1.GetEntries()):
       tree1.GetEntry(event)

       # get particle and hit branches
       number_particles= getattr (tree1,"number_particles")
       number_hits= getattr (tree1,"number_hits")
       particle_track_id= getattr (tree1,"particle_track_id")
       particle_parent_track_id= getattr (tree1,"particle_parent_track_id")
       particle_initial_x= getattr (tree1,"particle_initial_x")
       particle_initial_y= getattr (tree1,"particle_initial_y")
       particle_initial_z= getattr (tree1,"particle_initial_z")
       particle_initial_px= getattr (tree1,"particle_initial_px")
       particle_initial_py=getattr (tree1,"particle_initial_py")
       particle_initial_pz=getattr (tree1,"particle_initial_pz")
       hit_track_id=getattr (tree1,"hit_track_id")
       hit_start_x=getattr (tree1,"hit_start_x")
       hit_start_y=getattr (tree1,"hit_start_y")
       hit_start_z=getattr (tree1,"hit_start_z")
       hit_start_t=getattr (tree1,"hit_start_t")
       hit_end_x=getattr (tree1,"hit_end_x")
       hit_end_y=getattr (tree1,"hit_end_y")
       hit_end_z=getattr (tree1,"hit_end_z")
       hit_end_t=getattr (tree1,"hit_end_t")

       ## loop over all particles to get to the primary particle(s)
       for i in range(number_particles):
           # if primary particle
           if particle_parent_track_id[i]==0:
               electron_track_id = particle_track_id[i]

               # these are the values we are trying to reconstruct (inital position and momentum)
               electron_start_position = [particle_initial_x[i], particle_initial_y[i], particle_initial_z[i]]
               electron_start_momentum = [particle_initial_px[i], particle_initial_py[i], particle_initial_pz[i]]

               ## loop over all hits to get all hits associated with the primary electron_track_id
               hits_start = []
               hits_end = []
               k = 0
               for j in range (number_hits):
                   if hit_track_id[j] == electron_track_id:
                       # get position x,y,z of hit
                       # hits_start.append([hit_start_x[j], hit_start_y[j], hit_start_z[j], hit_start_t[j]])
                       hits_end.append([hit_end_x[j], hit_end_y[j], hit_end_z[j], hit_end_t[j]])
                       k += 1
               hits_end = np.array(hits_end)

               ## Make graphs of hits
               # define x,y,z data
               x = hits_end[:,0]
               y = hits_end[:,1]
               z = hits_end[:,2]

               #3D graph
               fig1 = plt.figure()
               plt.title('Electron hits 3d')
               ax1 = fig1.add_subplot(111, projection='3d')
               ax1.set_xlabel('$X$')
               ax1.set_ylabel('$Y$')
               ax1.set_zlabel('$Z$')
               #ax1.set_xlim([0,115])
               #ax1.set_ylim([0,115])
               #ax1.set_zlim([0,115])
               ax1.scatter(x,y,z,s=1)
               ax1.scatter(electron_start_position[0],electron_start_position[1],electron_start_position[2],s=2, color='red')

               #add line of non interfered trajectory
               electron_vacuum_point2 = np.add(electron_start_position, np.multiply(electron_start_momentum,100))
               ax1.plot(electron_start_position, electron_vacuum_point2)

               fig1.savefig('electron_path_3d1.png', bbox_inches='tight')   # Save graph as a png, to your current directory

               #2D graphs
               fig2, axs = plt.subplots(2,2)
               fig2.tight_layout()
               plt.title('Electron hits 2d')
               # x-y
               axs[0,0].set_xlabel('$X$')
               axs[0,0].set_ylabel('$Y$')
               axs[0,0].scatter(electron_start_position[0],electron_start_position[1],s=2, color='red')

               #axs[0,0].set_xlim([0,115])
               #axs[0,0].set_ylim([0,115])
               # x-z
               axs[0,1].set_xlabel('$X$')
               axs[0,1].set_ylabel('$Z$')
               axs[0,1].scatter(electron_start_position[0],electron_start_position[2],s=2, color='red')
               #axs[0,1].set_xlim([0,115])
               #axs[0,1].set_ylim([0,115])

               # y-z
               axs[1,0].set_xlabel('$Y$')
               axs[1,0].set_ylabel('$Z$')
               axs[1,0].scatter(electron_start_position[1],electron_start_position[2],s=2, color='red')
               #axs[1,0].set_xlim([0,115])
               #axs[1,0].set_ylim([0,115])


               # populate the graph with scatter points
               axs[0,0].scatter(x,y,s=1)
               axs[0,1].scatter(x,z,s=1)
               axs[1,0].scatter(y,z,s=1)
               fig2.delaxes(axs[1,1])

               fig2.savefig('electron_path_2d.png', bbox_inches='tight')

