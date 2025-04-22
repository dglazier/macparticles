
from matplotlib import pyplot as plt
import matplotlib
matplotlib.use('Agg')
import seaborn as sns
import numpy as np
import uproot
import awkward as ak

def calc4V(arr,start,M):
    Px = arr[:,start] * np.sin(arr[:,start+1]) * np.cos(arr[:,start+2])
    Py  = arr[:,start] * np.sin(arr[:,start+1]) * np.sin(arr[:,start+2])
    Pz = arr[:,start] * np.cos(arr[:,start+1])
    E =  np.sqrt(np.square(arr[:,start])+M**2)
    return (Px,Py,Pz,E)

def calcExc(arr,beamE):
    elM=0.000511
    pM=0.938
    phM=0

    el4V = calc4V(arr,0,elM)

    p4V = calc4V(arr,3,pM)

    ph4V = calc4V(arr,6,phM)
    
    px_m = -1.0*(el4V[0] + ph4V[0] + p4V[0])
    py_m = -1.0*(el4V[1] + ph4V[1] + p4V[1])
    pz_m = beamE - (el4V[2] + ph4V[2] + p4V[2])
    E_m = beamE + pM - (el4V[3]+ph4V[3]+p4V[3])
    MM2 = np.square(E_m) - (np.square(px_m) + np.square(py_m)+np.square(pz_m))
   

    Q2 = 2 * beamE * el4V[3] * (1. - el4V[2]/(np.sqrt(np.square(el4V[0]) + np.square(el4V[1])+np.square(el4V[2]))))
    

    nu = beamE - el4V[3]

    x = Q2 / (2*pM*nu)

    W = np.sqrt( pM**2 + 2 * pM * nu - Q2)

    t = -1*(np.square(p4V[3]-pM) - (np.square(p4V[0]) + np.square(p4V[1])+np.square(p4V[2])))

    MM2=MM2.reshape((MM2.shape[0],1))
    Q2=Q2.reshape((Q2.shape[0],1))
    t=t.reshape((t.shape[0],1))
    x=x.reshape((x.shape[0],1))
    W=W.reshape((W.shape[0],1))

    return np.hstack((MM2,Q2,t,x,W))

def histErrorBars(data, color, label,ax,rge):
    sumweights,errs,edges,bincenters=getHistErrorBars(data,rge)
    histo = ax.errorbar(bincenters, sumweights, errs, fmt='.',color=color, label=label,linewidth=3,markersize=25,capsize=7,elinewidth=5)
    return histo,sumweights,errs,edges

def getHistErrorBars(data, rge):
    sumweights, edges = np.histogram(data, range=rge,bins=500)
    errs = np.sqrt(sumweights)
    bincenters = []
    for i in range(len(sumweights)):
      bincenters.append( (edges[i+1]-edges[i])/2 + edges[i] )
    return sumweights,errs,edges,bincenters

def plotEffComp(all,gemc,fast,ranges,titles,names,units,print_dir,endName):

    for j in range(0,gemc.shape[1]):

      
      nall,errall,ball,bcall=getHistErrorBars(all[:,j],rge=(ranges[j][0],ranges[j][1]))
      nsig,errsig,bsig,bcsig=getHistErrorBars(gemc[:,j],rge=(ranges[j][0],ranges[j][1]))
      npred,errpred,bpred,bcpred=getHistErrorBars(fast[:,j],rge=(ranges[j][0],ranges[j][1]))

      effsig=[]
      effsig_err=[]
      effpred=[]
      effpred_err=[]
      plotloc=[]

      for i in range(len(nsig)):
        if (nall[i]!=0):
          e=nsig[i]/nall[i]
          effsig.append(e)
          if(nsig[i]==0):
            effsig_err.append(0)
          else:
            effsig_err.append( e*np.sqrt( np.square(errsig[i]/nsig[i])  +  np.square(errall[i]/nall[i]) ) )#error for ratio

          e=npred[i]/nall[i]
          effpred.append(e)
          if(npred[i]==0):
            effpred_err.append(0)
          else:
            effpred_err.append( e*np.sqrt( np.square(errpred[i]/npred[i])  +  np.square(errall[i]/nall[i]) ) )#error for ratio

          plotloc.append( (bsig[i+1]-bsig[i])/2 + bsig[i] )

      res=[]
      res_err=[]
      plotlocb=[]
      for i in range(len(effsig)):
        if (effpred[i]!=0) or (effsig[i]!=0):
          std=np.sqrt(effsig_err[i]**2 + effpred_err[i]**2) #sum of squares of sqrt(nsig) and sqrt(npred)
          res.append((effsig[i]-effpred[i])/std)
          res_err.append(1)
          plotlocb.append( plotloc[i] )
        

      limUp=0.08
      if (names[j]=='Q2') or (names[j]=='x'):
        limUp=0.3

      fig, axs = plt.subplots(2, 1, figsize=(20, 20), sharex=True, sharey=False,tight_layout=True,height_ratios=[2, 1])

      
      axs[0].errorbar(x=plotloc, y=effsig, yerr=effsig_err,fmt='s', color='royalblue', label='GEMC',ms=10,capsize=7,elinewidth=3)
      axs[0].errorbar(x=plotloc, y=effpred, yerr=effpred_err,fmt='s', color='firebrick', label='FastMC',ms=10,capsize=7,elinewidth=3)
      axs[0].legend(loc='upper right')
      ymin, ymax = axs[0].get_ylim()
      axs[0].set_ylim(0, limUp)
      axs[0].set_title('Acceptance vs '+titles[j])
      axs[0].set_ylabel('Acceptance')

      axs[1].errorbar(x=plotlocb, y=res, yerr=res_err,fmt='s', color='mediumorchid',ms=10,capsize=7,elinewidth=3)
      axs[1].axhline(y = 0.0, color = 'black', linestyle = '--')#,label='1') 
      axs[1].axhline(y = -1.0, color = 'grey', linestyle = '--')#,label='1') 
      axs[1].axhline(y = 1.0, color = 'grey', linestyle = '--')#,label='1') 
      axs[1].axhline(y = -2.0, color = 'silver', linestyle = '--')#,label='1') 
      axs[1].axhline(y = 2.0, color = 'silver', linestyle = '--')#,label='1') 
      axs[1].set_ylim(-5, 5)
      axs[1].set_ylabel('Pull')
      plt.xlabel(titles[j]+' '+units[j])
      plt.savefig(print_dir+'Eff_'+names[j]+endName+'.png')

def plotComp(gemc,fast,ranges,titles,names,units,print_dir,endName,all=np.zeros((1,1))):

    for j in range(0,gemc.shape[1]):
      fig, axs = plt.subplots(2, 1, figsize=(20, 20), sharex=True, sharey=False,tight_layout=True,height_ratios=[2, 1])

      if(all.shape[0]>1):
        hall,nall,errall,ball=histErrorBars(all[:,j], color="black", label="Generated",ax=axs[0],rge=(ranges[j][0],ranges[j][1]))
      hsig,nsig,errsig,bsig=histErrorBars(gemc[:,j], color="royalblue", label="GEMC",ax=axs[0],rge=(ranges[j][0],ranges[j][1]))
      hpred,npred,errpred,bpred=histErrorBars(fast[:,j], color="firebrick", label="FastMC",ax=axs[0],rge=(ranges[j][0],ranges[j][1]))
      axs[0].legend(loc='upper right')

      ymin, ymax = axs[0].get_ylim()
      axs[0].set_ylim(0, ymax * 1.35)
      axs[0].set_title(titles[j])

      res=[]
      res_err=[]
      plotloc=[]
      for i in range(len(nsig)):
        if (npred[i]!=0) or (nsig[i]!=0):
          #res.append(nsig[i]/npred[i])
          #e=res[i]*np.sqrt( np.square((np.sqrt(nsig[i])/nsig[i])) +  np.square((np.sqrt(npred[i])/npred[i])) ) #error for ratio
          std=np.sqrt(errsig[i]**2 + errpred[i]**2) #sum of squares of sqrt(nsig) and sqrt(npred)
          res.append((nsig[i]-npred[i])/std)
          res_err.append(1)
          plotloc.append( (bsig[i+1]-bsig[i])/2 + bsig[i] )
        
      axs[1].errorbar(x=plotloc, y=res, yerr=res_err,fmt='s', color='mediumorchid',ms=10,capsize=7,elinewidth=3)
      axs[1].axhline(y = 0.0, color = 'black', linestyle = '--')#,label='1') 
      axs[1].axhline(y = -1.0, color = 'grey', linestyle = '--')#,label='1') 
      axs[1].axhline(y = 1.0, color = 'grey', linestyle = '--')#,label='1') 
      axs[1].axhline(y = -2.0, color = 'silver', linestyle = '--')#,label='1') 
      axs[1].axhline(y = 2.0, color = 'silver', linestyle = '--')#,label='1') 
      axs[1].set_ylim(-5, 5)
      axs[1].set_ylabel('Pull')
      plt.xlabel(titles[j]+' '+units[j])
      plt.savefig(print_dir+'Comp_'+names[j]+endName+'.png')

def plot2DComp(gemc,fast,v1,v2,titles,units,name,ranges,print_dir,endName,vmax=100):

    nBins=75

    h1err, xedges1err, yedges1err, image1err=plt.hist2d(gemc[:,v1], gemc[:,v2], range=(ranges[v1],ranges[v2]), bins=((nBins,nBins)))
    h2err, xedges2err, yedges2err, image2err=plt.hist2d(fast[:,v1], fast[:,v2], range=(ranges[v1],ranges[v2]), bins=((nBins,nBins)))

    fig, axs = plt.subplots(1, 3, figsize=(45, 15), sharex=True, sharey=True,
                          tight_layout=True)


    #cmin=1
    h1, xedges1, yedges1, image1=axs[0].hist2d(gemc[:,v1], gemc[:,v2], range=(ranges[v1],ranges[v2]), bins=((nBins,nBins)),vmin=-10,vmax=vmax)#, norm=matplotlib.colors.LogNorm()) ,,vmin=-10,vmax=10
    
    axs[0].set_xlabel(titles[v1]+units[v1])
    axs[0].set_ylabel(titles[v2]+units[v2])
    axs[0].set_title("GEMC")
    fig.colorbar(image1, ax=axs[0])

    # As well as define normalization of the colors
    h2, xedges2, yedges2, image2=axs[1].hist2d(fast[:,v1], fast[:,v2], range=(ranges[v1],ranges[v2]), bins=((nBins,nBins)),vmin=-10,vmax=vmax)#, norm=matplotlib.colors.LogNorm())
    
    axs[1].set_xlabel(titles[v1]+units[v1])
    axs[1].set_ylabel(titles[v2]+units[v2])
    axs[1].set_title("FastMC")
    fig.colorbar(image2, ax=axs[1])

    res=(h1-h2).T
    std=np.sqrt(np.abs(h1err) + np.abs(h2err)).T #sum of squares of sqrt(h1err) and sqrt(h2err)
    stdb=std.copy()
    stdb[stdb==0]=1
    res=res/stdb
    res[std==0]=0

    #print(h1)
    cmesh=axs[2].pcolormesh(xedges1, yedges1, res,vmin=-3,vmax=3)
    axs[2].set_xlabel(titles[v1]+units[v1])
    axs[2].set_ylabel(titles[v2]+units[v2])
    axs[2].set_title('Pull')
    fig.colorbar(cmesh, ax=axs[2])

    plt.savefig(print_dir+name+endName+'.png', bbox_inches="tight")

plt.rcParams.update({'font.size': 40,
                    #'font.family':  'Times New Roman',
                    'legend.edgecolor': 'white',
                    'xtick.minor.visible': True,
                    'ytick.minor.visible': True,
                    'xtick.major.size':15,
                    'xtick.minor.size':10,
                    'ytick.major.size':15,
                    'ytick.minor.size':10,
                    'xtick.major.width':3,
                    'xtick.minor.width':3,
                    'ytick.major.width':3,
                    'ytick.minor.width':3,
                    'axes.linewidth' : 3,
                    'figure.max_open_warning':200,
                    'lines.linewidth' : 5})




ranges=[(0,10),(0,0.6),(0,2*3.15),(0,1.5),(0,1.5),(0,2*3.15),(0,10),(0,0.5),(0,2*3.15)]
titles=['$e^-$ P',r'$e^-$ $\theta$',r'$e^-$ $\phi$','p P',r'p $\theta$',r'p $\phi$','$\gamma$ P',r'$\gamma$ $\theta$',r'$\gamma$ $\phi$']
names=['elP','elTheta','elPhi','pP','pTheta','pPhi','phP','phTheta','phPhi']

delta_ranges=[(-1,1),(-0.1,0.1),(-0.1,0.1),(-0.5,0.5),(-0,0.1),(-0.1,0.1),(-1,1),(-0.1,0.1),(-0.1,0.1)]
delta_names=['delta_elP','delta_elTheta','delta_elPhi','delta_pP','delta_pTheta','delta_pPhi','delta_phP','delta_phTheta','delta_phPhi']
delta_titles=['$e^-$ $\Delta$P',r'$e^-$ $\Delta\theta$',r'$e^-$ $\Delta\phi$','p $\Delta$P',r'p $\Delta\theta$',r'p $\Delta\phi$','$\gamma$ $\Delta$P',r'$\gamma$ $\Delta\theta$',r'$\gamma$ $\Delta\phi$']

units=['[GeV]','[Radians]','[Radians]','[GeV]','[Radians]','[Radians]','[GeV]','[Radians]','[Radians]']

print_dir='pred_output/testing_plots/'
endName='_0nA_elPs2'

gemc_file = uproot.open('all_events_test.root')  # replace with your file name
fast_file = uproot.open('pred_output/all/predictions.root')  # replace with your file name
fast_acc_file = uproot.open('pred_output/all/simulation_acceptances.root')  # replace with your file name

gemc_tree = gemc_file["tree"]  # adjust the tree name if different
fast_tree = fast_file["recon"]
fast_acc_tree = fast_acc_file["acceptance"]

variables = ["truel_p", "truel_theta", "truel_phi", "trup1_p", "trup1_theta", "trup1_phi", "trup2_p", "trup2_theta", "trup2_phi","recel_p", "recel_theta", "recel_phi", "recp1_p", "recp1_theta", "recp1_phi", "recp2_p", "recp2_theta", "recp2_phi","accepted"]
variables2 = ["recel_p", "recel_theta", "recel_phi", "recp1_p", "recp1_theta", "recp1_phi", "recp2_p", "recp2_theta", "recp2_phi"]

gemc_data = gemc_tree.arrays(variables, library="np")
fast_data = fast_tree.arrays(variables2, library="np")
fast_acc_data = fast_acc_tree.arrays(["accept_all"], library="np")

gemc = np.column_stack([gemc_data[var] for var in variables])
fast = np.column_stack([fast_data[var] for var in variables2])
fast_acc = np.column_stack([fast_acc_data["accept_all"]])
fast_acc=fast_acc.reshape((fast_acc.shape[0]))

gen=gemc[gemc[:,-1]==0][:,:9]
gemc=gemc[gemc[:,-1]==1]

gemc_true=gemc[:,:9]
gemc_rec=gemc[:,9:18]
gemc_res=gemc_true - gemc_rec

fast_rec=fast[fast_acc==1]
#fast_res=gemc_true - fast_rec

#cut edge in P
gemc_rec=gemc_rec[gemc_rec[:,0]>2]
fast_rec=fast_rec[fast_rec[:,0]>2]
gen=gen[gen[:,0]>2]

plotComp(gemc_rec,fast_rec,ranges,titles,names,units,print_dir,endName,all=gen)
plotComp(gemc_rec,fast_rec,ranges,titles,names,units,print_dir,'_ReconVars'+endName)

for i in range(0,3):
  pVarNb=i*3+0
  thetaVarNb=i*3+1
  phiVarNb=i*3+2
  plot2DComp(gemc_rec,fast_rec,pVarNb,thetaVarNb,titles,units,names[pVarNb]+"vs"+names[thetaVarNb],ranges,print_dir,endName,vmax=100)
  plot2DComp(gemc_rec,fast_rec,pVarNb,phiVarNb,titles,units,names[pVarNb]+"vs"+names[phiVarNb],ranges,print_dir,endName,vmax=100)
  plot2DComp(gemc_rec,fast_rec,thetaVarNb,phiVarNb,titles,units,names[thetaVarNb]+"vs"+names[phiVarNb],ranges,print_dir,endName,vmax=100)

#MM2,Q2,t,x,W
gen_exc=calcExc(gen,10.6)
gemc_exc=calcExc(gemc_rec,10.6)
fast_exc=calcExc(fast_rec,10.6)

exc_ranges=[(-0.01,0.01),(0.5,5.5),(0,1.25),(0,0.8),(1.5,4.5)]
exc_titles=['MM$^2$','Q$^2$','-t','x','W']
exc_names=['MM2','Q2','t','x','W']
exc_units=['[GeV$^2$]','[GeV$^2$]','[GeV$^2$]','[GeV]','[GeV]']

plotComp(gemc_exc,fast_exc,exc_ranges,exc_titles,exc_names,exc_units,print_dir,endName,all=gen_exc)
plotComp(gemc_exc,fast_exc,exc_ranges,exc_titles,exc_names,exc_units,print_dir,'_ReconVars'+endName)

plotEffComp(gen_exc,gemc_exc,fast_exc,exc_ranges,exc_titles,exc_names,exc_units,print_dir,endName)


