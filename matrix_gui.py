import tkinter as tk
from tkinter import ttk
from tkinter import messagebox
import networkx as nx
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import csv

# Global variables
units = []
connections = []
graph = nx.DiGraph()

# Initialize the graph with some sample units and connections
units.append(0)
units.append(1)
units.append(2)
connections.append((0, 1))
connections.append((1, 2))
graph.add_nodes_from(units)
graph.add_edges_from(connections)

layouts = {
    "Fruchterman-Reingold": nx.fruchterman_reingold_layout,
    "Circular": nx.circular_layout,
    "Random": nx.random_layout,
    "Spectral": nx.spectral_layout,
    "Spring": nx.spring_layout,
}

class UnitConnectionApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Draw Units and Connections")

        self.unit_spinbox = tk.Spinbox(root, width=10, from_=1.0, to=100.0)
        self.unit_spinbox.grid(row=0, column=0, padx=5, pady=5)

        self.add_button = tk.Button(root, text="Add Units", command=self.add_unit)
        self.add_button.grid(row=0, column=1, padx=5, pady=5)

        self.from_unit_label = tk.Label(root, text="From:")
        self.from_unit_label.grid(row=1, column=0, padx=5, pady=5)
        
        self.from_unit_combobox = tk.ttk.Combobox(root, values=units)
        self.from_unit_combobox.grid(row=1, column=1, padx=5, pady=5)   
        
        self.connect_button = tk.Button(root, text="Connect Units", command=self.connect_units)
        self.connect_button.grid(row=1, column=2, padx=5, pady=5)

        self.to_unit_label = tk.Label(root, text="To:")
        self.to_unit_label.grid(row=2, column=0, padx=5, pady=5)
        
        self.to_unit_combobox = tk.ttk.Combobox(root, values=units)
        self.to_unit_combobox.grid(row=2, column=1, padx=5, pady=5)
        
        self.disconnect_button = tk.Button(root, text="Disonnect Units", command=self.disconnect_units)
        self.disconnect_button.grid(row=2, column=2, padx=5, pady=5)

        self.layouts_combobox = tk.ttk.Combobox(root, values=list(layouts.keys()))
        self.layouts_combobox.current(1)
        self.layouts_combobox.grid(row=3, column=0, padx=5, pady=5)  
        
        self.refresh_button = tk.Button(root, text="Refresh", command=self.draw_graph)
        self.refresh_button.grid(row=3, column=1, padx=5, pady=5)
        
        self.save_button = tk.Button(root, text="Save", command=self.graph_to_adjacency_matrix)
        self.save_button.grid(row=3, column=2, padx=5, pady=5)
        
        self.load_button = tk.Button(root, text="Load", command=self.adjacency_matrix_to_graph)
        self.load_button.grid(row=0, column=2, padx=5, pady=5)

        self.figure = plt.Figure(figsize=(15, 8), dpi=100)
        self.ax = self.figure.add_subplot(111)
        self.canvas = FigureCanvasTkAgg(self.figure, master=root)
        self.canvas.get_tk_widget().grid(row=4, columnspan=3, padx=5, pady=5)

        self.draw_graph()

    def add_unit(self):
        if int(self.unit_spinbox.get()) > 0:
            graph.remove_nodes_from(units)
            graph.remove_edges_from(connections)
            for i in range(int(self.unit_spinbox.get())):
                print(i)
                new_unit = units[-1]+1
                prew_unit = units[-1]
                units.append(new_unit)
                connections.append((prew_unit, new_unit))
            graph.add_nodes_from(units)
            graph.add_edges_from(connections)
            self.from_unit_combobox["values"] = units
            self.to_unit_combobox["values"] = units
            #self.unit_spinbox.delete(0, tk.END)
            self.draw_graph()
        else:
            messagebox.showwarning("Warning", "Please enter positive value!")
                
        #if not self.unit_spinbox.get().strip():
        #prew_unit = new_unit-1
        #connections.append((prew_unit, new_unit))
        #elif int(self.unit_spinbox.get().strip()) > 0:
            #prew_unit = int(self.unit_spinbox.get().strip())
            #connections.append((prew_unit, new_unit))
            
            

    def connect_units(self):
        from_unit = int(self.from_unit_combobox.get())
        to_unit = int(self.to_unit_combobox.get())

        if from_unit in units and to_unit in units and from_unit != to_unit:
            graph.remove_nodes_from(units)
            graph.remove_edges_from(connections)
            connections.append((from_unit, to_unit))
            #graph.add_nodes_from(units)
            graph.add_edges_from(connections)
            self.draw_graph()
        else:
            messagebox.showwarning("Warning", "Please select two different units to connect.")
            
    def disconnect_units(self):
        from_unit = int(self.from_unit_combobox.get())
        to_unit = int(self.to_unit_combobox.get())

        if from_unit in units and to_unit in units and from_unit != to_unit and (from_unit, to_unit) in connections:
            graph.remove_nodes_from(units)
            graph.remove_edges_from(connections)
            connections.remove((from_unit, to_unit))
            #graph.add_nodes_from(units)
            graph.add_edges_from(connections)
            self.draw_graph()
        else:
            messagebox.showwarning("Warning", "Please select two different units to connect.")

    def draw_graph(self):
        print(units, "\n", connections, "\n\n\n")
        values = {node: node_value for node, node_value in zip(graph.nodes, np.cos(units))}
        min_value, max_value = min(values.values()), max(values.values())
        norm = plt.Normalize(min_value, max_value)
        cmap = plt.cm.viridis
        node_colors = [cmap(norm(value)) for value in values.values()]
        self.ax.clear()
        pos = layouts[self.layouts_combobox.get()](graph)
        nx.draw(graph, pos, with_labels=True, node_color=node_colors, font_size = 5+100//len(units), node_size = 10 + 3000/len(units), edge_color="gray", ax=self.ax) #node_color="lightblue",
        self.canvas.draw()
        
    def graph_to_adjacency_matrix(self):
        nodes = list(graph.nodes())
        adj_matrix = np.zeros((len(nodes), len(nodes)))

        for i, node1 in enumerate(nodes):
            for j, node2 in enumerate(nodes):
                if graph.has_edge(node1, node2):
                    adj_matrix[i, j] = 1
        with open('coupling_matrix.dat', 'w', newline='') as datfile:
            writer = csv.writer(datfile, delimiter='\t')
            writer.writerows(adj_matrix.tolist())
        print( adj_matrix)
        
    def adjacency_matrix_to_graph(self):
        adj_matrix = np.loadtxt('coupling_matrix.dat')
        #graph.remove_nodes_from(units)
        #graph.remove_edges_from(connections)
        global graph, units, connections
        graph = nx.from_numpy_array(adj_matrix, create_using=nx.DiGraph)
        #units = []
        #connections = []
        units = list(graph.nodes())
        connections = list(graph.edges())
        print(graph, units, connections)
        self.draw_graph()

        
if __name__ == "__main__":
    root = tk.Tk()
    app = UnitConnectionApp(root)
    root.mainloop()
