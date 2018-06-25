import SimpleITK as sitk
import matplotlib.pyplot as plt
import ipywidgets as widgets
from IPython.display import display
import numpy as np
import random
import matplotlib.cm as cm
from matplotlib.ticker import MaxNLocator

class PairedPointDataManipulation(object):
    def __init__(self, transform=sitk.Euler2DTransform(), figure_size=(8,6)):
        self.figure_size = figure_size
        self.fiducial_indexes = []
        self.target_indexes = []
        self.original_fiducials = []
        self.original_targets = []
        self.fixed_fiducial_indexes = []
        self.fixed_target_indexes = []
        self.transform = transform
        self.FLE_errors = []
        self.FRE_errors = []
        self.TRE_errors = []
        self.scale = 100

        self.ui = self.create_ui()
        display(self.ui)
        
        # Create a figure. 
        self.fig, self.axes = plt.subplots(1,1,True, True, figsize=self.figure_size)
        
        self.fig.canvas.mpl_connect('button_press_event', self.on_press)
        self.fig.canvas.mpl_connect('motion_notify_event', self.on_motion)
        self.fig.canvas.mpl_connect('button_release_event', self.on_release)
        
        self.update_display()
        

    def create_ui(self):
        # Create the active UI components. Height and width are specified in 'em' units. This is
        # a html size specification, size relative to current font size.
        self.viewing_checkbox = widgets.RadioButtons(description= 'Mode:', 
                                                     options= ['edit', 'drag', 'rotate'], 
                                                     value = 'edit')
        self.viewing_checkbox.observe(self.draw_centroid)

        self.noise_button = widgets.Button(description= 'Add Noise', 
                                              width= '7em', 
                                              height= '3em') 
        self.noise_button.on_click(self.noise)

        self.outlier_button = widgets.Button(description= 'Add Outlier', 
                                              width= '7em', 
                                              height= '3em') 
        self.outlier_button.on_click(self.outlier)
        
        self.bias1_button = widgets.Button(description= 'Bias (FRE<TRE)', 
                                               width= '7em', 
                                               height= '3em')
        self.bias1_button.on_click(self.bias_1)

        self.bias2_button = widgets.Button(description= 'Bias (FRE>TRE)', 
                                              width= '7em', 
                                              height= '3em') 
        self.bias2_button.on_click(self.bias_2)
        
        self.clear_fiducial_button = widgets.Button(description= 'Clear Fiducials', 
                                               width= '7em', 
                                               height= '3em')
        self.clear_fiducial_button.on_click(self.clear_fiducials)

        self.clear_target_button = widgets.Button(description= 'Clear Targets', 
                                               width= '7em', 
                                               height= '3em')
        self.clear_target_button.on_click(self.clear_targets)
        
        self.reset_button = widgets.Button(description= 'Reset', 
                                               width= '7em', 
                                               height= '3em')
        self.reset_button.on_click(self.reset_indexes)
        
        self.register_button = widgets.Button(description= 'Register', 
                                               width= '7em', 
                                               height= '3em')
        self.register_button.on_click(self.register)

        
        # Layout of UI components. This is pure ugliness because we are not using a UI toolkit. Layout is done
        # using the box widget and padding so that the visible UI components are spaced nicely.
        bx0 = widgets.Box(padding = 2, children = [self.viewing_checkbox])
        bx1 = widgets.Box(padding = 15, children = [self.noise_button])
        bx2 = widgets.Box(padding = 15, children = [self.outlier_button])
        bx3 = widgets.Box(padding = 15, children = [self.bias1_button])
        bx4 = widgets.Box(padding = 15, children = [self.bias2_button])
        bx5 = widgets.Box(padding = 15, children = [self.clear_fiducial_button])
        bx6 = widgets.Box(padding = 15, children = [self.clear_target_button])
        bx7 = widgets.Box(padding = 15, children = [self.reset_button])
        bx8 = widgets.Box(padding = 15, children = [self.register_button])
        return widgets.HBox(children=[bx0, widgets.VBox(children=[widgets.HBox([bx1, bx2, bx3, bx4]), widgets.HBox(children=[bx5, bx6, bx7, bx8])])])

    
    def update_display(self):
        # Draw the image and localized fiducials and targets.
        self.axes.clear()
        
        self.fixed_fiducials = []
        self.fixed_targets = []
        self.fiducials = []
        self.targets = []
        for a, b in self.fixed_fiducial_indexes:
            self.fixed_fiducials += self.axes.plot(a, b, 'x', markersize=6, color="#ff8888")
        for a, b in self.fixed_target_indexes:
            self.fixed_targets += self.axes.plot(a, b, 'x', markersize=6, color="#88ff88")
        for a, b in self.fiducial_indexes:
            self.fiducials += self.axes.plot(a, b, '+', markersize=8, color="#ffdddd")
        for a, b in self.target_indexes:
            self.targets += self.axes.plot(a, b, '+', markersize=8, color="#ddffdd")
        if self.viewing_checkbox.value == 'rotate' and (self.target_indexes or self.fiducial_indexes):
            x, y = zip(*(self.target_indexes+self.fiducial_indexes))
            self.centroid, = self.axes.plot((sum(x))/(len(self.target_indexes)+len(self.fiducial_indexes)),
                                           (sum(y))/(len(self.target_indexes)+len(self.fiducial_indexes)),
                                           '.', markersize=4, color="#58d8ff")
        
        self.axes.set_title('Registration Error Demonstration')
        self.axes.get_xaxis().set_visible(False)
        self.axes.get_yaxis().set_visible(False)
        
        self.axes.set_facecolor((0, 0, 0))
        
        # Set the zoom factor back to what it was before we cleared the axes, and rendered our data.
        self.axes.set_xlim([0, self.scale])
        self.axes.set_ylim([0, self.scale])

        self.fig.canvas.draw_idle()
    
    
    def draw_centroid(self, button):
        self.update_display()

    def noise(self, button):
        self.fiducial_indexes = []
        for fiducial in self.fiducials:
            self.fiducial_indexes += [((fiducial.get_xdata()+random.uniform(-0.01*self.scale, 0.01*self.scale))[0],
                                       (fiducial.get_ydata()+random.uniform(-0.01*self.scale, 0.01*self.scale))[0])]
        self.update_display()

    def outlier(self, button):
        index = random.randrange(len(self.fiducial_indexes))
        self.fiducial_indexes[index] = (max(min(self.fiducial_indexes[index][0]+0.06*self.scale, self.scale), 0),
                                        max(min(self.fiducial_indexes[index][1]+0.06*self.scale, self.scale), 0))
        self.update_display()

    def bias_1(self, button):
        self.fiducial_indexes = []
        for fiducial in self.fiducials:
            self.fiducial_indexes += [((fiducial.get_xdata()+0.015*self.scale)[0],
                                       (fiducial.get_ydata()+0.015*self.scale)[0])]
        self.update_display()
    
    def bias_2(self, button):
        pol=1
        self.fiducial_indexes = []
        for fiducial in self.fiducials:
            self.fiducial_indexes += [((fiducial.get_xdata()+0.015*pol*self.scale)[0],
                                       (fiducial.get_ydata()+0.015*pol*self.scale)[0])]
            pol*=-1
        self.update_display()
    
    def clear_fiducials(self, button):
        self.fixed_fiducial_indexes = []
        self.fiducial_indexes = []
        self.update_display()

    def clear_targets(self, button):
        self.fixed_target_indexes = []
        self.target_indexes = []
        self.update_display()

    def reset_indexes(self, button):
        self.fiducial_indexes = self.fixed_fiducial_indexes[:]
        self.target_indexes = self.fixed_target_indexes[:]
        self.update_display()
        
    def register(self, button):
        self.original_fiducials = self.fiducial_indexes
        self.original_targets = self.target_indexes

        fixed_image_points = [c for p in self.fixed_fiducial_indexes for c in p]
        moving_image_points = [c for p in self.fiducial_indexes for c in p]

        transform = sitk.LandmarkBasedTransformInitializer(self.transform, fixed_image_points, moving_image_points)
        
        self.FLE_errors = registration_errors(sitk.Transform(2, sitk.sitkIdentity), self.fixed_fiducial_indexes, self.fiducial_indexes)
        self.FRE_errors = registration_errors(transform, self.fixed_fiducial_indexes, self.fiducial_indexes)
        self.TRE_errors = registration_errors(transform, self.fixed_target_indexes, self.target_indexes)
        
        invert = transform.GetInverse()
        
        self.fiducial_indexes = []
        for fiducial in self.fiducials:
            self.fiducial_indexes.append(invert.TransformPoint((fiducial.get_xdata()[0],fiducial.get_ydata()[0])))
        
        self.target_indexes = []            
        for target in self.targets:
            self.target_indexes.append(invert.TransformPoint((target.get_xdata()[0],target.get_ydata()[0])))
            
        self.update_display()
    
    
    def on_press(self, event):
        if self.viewing_checkbox.value == 'edit':
            if self.target_indexes==self.fixed_target_indexes and self.fiducial_indexes==self.fixed_fiducial_indexes:
                if event.button ==1: #left click
                    if event.inaxes==self.axes:
                        self.fiducial_indexes.append((event.xdata, event.ydata))
                        self.fixed_fiducial_indexes.append((event.xdata, event.ydata))
                        self.update_display()
                elif event.button ==3: #right click
                    if event.inaxes==self.axes:
                        self.target_indexes.append((event.xdata, event.ydata))
                        self.fixed_target_indexes.append((event.xdata, event.ydata))
                        self.update_display()
        elif event.button == 1: #left click
            if event.inaxes==self.axes:
                if self.viewing_checkbox.value == 'drag':
                    self.previousx = event.xdata
                    self.previousy = event.ydata
                elif self.viewing_checkbox.value == 'rotate' and (self.target_indexes or self.fiducial_indexes):
                    self.previous = [event.xdata - self.centroid.get_xdata(),
                                     event.ydata - self.centroid.get_ydata()]
                    
    def on_motion(self, event):
        if event.button == 1: #left click
            if self.viewing_checkbox.value == 'drag':
                for fiducial in self.fiducials:
                    fiducial.set_data(fiducial.get_xdata()+event.xdata-self.previousx,
                                      fiducial.get_ydata()+event.ydata-self.previousy)
                for target in self.targets:
                    target.set_data(target.get_xdata()+event.xdata-self.previousx,
                                    target.get_ydata()+event.ydata-self.previousy)
                self.previousx = event.xdata
                self.previousy = event.ydata
                self.fig.canvas.draw_idle()
                self.fig.canvas.flush_events()
            elif self.viewing_checkbox.value == 'rotate' and (self.target_indexes or self.fiducial_indexes):
                ox = self.centroid.get_xdata()
                oy = self.centroid.get_ydata()
                v1 = self.previous
                v2 = [event.xdata-ox, event.ydata-oy]
                
                cosang = v1[0]*v2[0]+v1[1]*v2[1]                
                sinang = v1[0]*v2[1]-v1[1]*v2[0]
                angle = np.arctan2(sinang, cosang)                

                for fiducial in self.fiducials:
                    px = fiducial.get_xdata()
                    py = fiducial.get_ydata()
                    fiducial.set_data(ox + np.cos(angle) * (px - ox) - np.sin(angle) * (py - oy),
                                      oy + np.sin(angle) * (px - ox) + np.cos(angle) * (py - oy))
                for target in self.targets:
                    px = target.get_xdata()
                    py = target.get_ydata()
                    target.set_data(ox + np.cos(angle) * (px - ox) - np.sin(angle) * (py - oy),
                                    oy + np.sin(angle) * (px - ox) + np.cos(angle) * (py - oy))
                self.previous = v2
                self.fig.canvas.draw_idle()
                self.fig.canvas.flush_events()
                    
    def on_release(self, event):
        if event.button == 1: #left click
            if self.viewing_checkbox.value == 'drag' or self.viewing_checkbox.value == 'rotate':
                self.fiducial_indexes = []
                self.target_indexes = []
                for fiducial in self.fiducials:
                    self.fiducial_indexes += [(fiducial.get_xdata()[0], fiducial.get_ydata()[0])]
                for target in self.targets:
                    self.target_indexes += [(target.get_xdata()[0], target.get_ydata()[0])]
                    
                self.update_display()
        

    def get_fixed_fiducials(self):
        return self.fixed_fiducial_indexes
        
    def get_fiducials(self):
        return self.fiducial_indexes

    def get_original_fiducials(self):
        if len(self.original_fiducials)==len(self.fixed_fiducial_indexes):
            return self.original_fiducials
        else:
            return self.fiducial_indexes

    def set_fiducials(self, fiducials):
        self.fixed_fiducial_indexes = fiducials[:]
        self.fiducial_indexes = fiducials[:]
        self.update_display()
        
        
    def get_fixed_targets(self):
        return self.fixed_target_indexes

    def get_targets(self):
        return self.target_indexes

    def get_original_targets(self):
        if len(self.original_targets)==len(self.fixed_target_indexes):
            return self.original_targets
        else:
            return self.target_indexes

    def set_targets(self, targets):
        self.fixed_target_indexes = targets[:]
        self.target_indexes = targets[:]
        self.update_display()

def display_errors(fixed_fiducial_target_indexes, FLE_errors, FRE_errors, TRE_errors,
                   min_err= None, max_err=None, title="Registration Errors"):
    if not min_err:
        min_err = min(FRE_errors[2], TRE_errors[2])
    if not max_err:
        max_err = max(FRE_errors[3], TRE_errors[3])

    print("Mean FLE %.6f\t STD FLE %.6f\t Min FLE %.6f\t Max FLE %.6f" %FLE_errors[0:4])
    print("Mean FRE %.6f\t STD FRE %.6f\t Min FRE %.6f\t Max FRE %.6f" %FRE_errors[0:4])
    print("Mean TRE %.6f\t STD TRE %.6f\t Min TRE %.6f\t Max TRE %.6f" %TRE_errors[0:4])
    fig = plt.figure(figsize=(9, 3.5), num=title)
    ax1 = plt.subplot(1, 2, 1)
    ax1.set_title('Histogram')
    ax1.yaxis.set_major_locator(MaxNLocator(integer=True))
    bins = np.linspace(min(FLE_errors[2], FRE_errors[2], TRE_errors[2]), max(FLE_errors[3], FRE_errors[3], TRE_errors[3]), 20)
    plt.hist(FLE_errors[4], bins, alpha=0.3, label='FLE')
    plt.hist(FRE_errors[4], bins, alpha=0.3, label='FRE')
    plt.hist(TRE_errors[4], bins, alpha=0.3, label='TRE')
    plt.legend(loc='upper right')

    ax2 = plt.subplot(1, 2, 2)
    ax2.get_xaxis().set_visible(False)
    ax2.get_yaxis().set_visible(False)
    ax2.set_facecolor((0.8, 0.8, 0.8))
    ax2.set_title('Colormap')
    collection = ax2.scatter(list(np.array(fixed_fiducial_target_indexes).T)[0],
                            list(np.array(fixed_fiducial_target_indexes).T)[1],
                            marker = 'o',
                            c = FRE_errors[4]+TRE_errors[4],
                            vmin = min_err,
                            vmax = max_err,
                            cmap = cm.hot)
    plt.colorbar(collection, shrink=0.8)
    plt.show()

def registration_errors(tx, reference_fixed_point_list, reference_moving_point_list,
                        min_err= None, max_err=None, figure_size=(8,6)):
  """
  Distances between points transformed by the given transformation and their
  location in another coordinate system. When the points are only used to 
  evaluate registration accuracy (not used in the registration) this is the 
  Target Registration Error (TRE).
  
  Args:
      tx (SimpleITK.Transform): The transform we want to evaluate.
      reference_fixed_point_list (list(tuple-like)): Points in fixed image 
                                                     cooredinate system.
      reference_moving_point_list (list(tuple-like)): Points in moving image 
                                                      cooredinate system.
      display_errors (boolean): Display a 3D figure with the points from 
                                reference_fixed_point_list color corresponding
                                to the error.
      min_err, max_err (float): color range is linearly stretched between min_err 
                                and max_err. If these values are not given then
                                the range of errors computed from the data is used.
      figure_size (tuple): Figure size in inches.
  Returns:
   (mean, std, min, max, errors) (float, float, float, float, [float]): 
    TRE statistics and original TREs.
  """
  transformed_fixed_point_list = [tx.TransformPoint(p) for p in reference_fixed_point_list]

  errors = [linalg.norm(np.array(p_fixed) -  np.array(p_moving))
            for p_fixed,p_moving in zip(transformed_fixed_point_list, reference_moving_point_list)]
  min_errors = np.min(errors)
  max_errors = np.max(errors)

  return (np.mean(errors), np.std(errors), min_errors, max_errors, errors) 