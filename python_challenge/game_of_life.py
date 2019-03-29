import tkinter as tk
import numpy as np
import time
import json


class GameOfLifeSimulator:
	def set_grid_params(self,grid_size,game_speed):
		self.grid_size = grid_size * 10
		self.game_speed = game_speed
		self.game_grid = np.zeros((grid_size,grid_size))



	def create_grid(self,event=None):
		w = self.canvas.winfo_width() 
		h = self.canvas.winfo_height() 
		self.canvas.delete('grid_line') 

		for i in range(0, w, 10):
			self.canvas.create_line([(i, 0), (i, h)], tag='grid_line',fill="#e5e5e5")

		for i in range(0, h, 10):
			self.canvas.create_line([(0, i), (w, i)], tag='grid_line',fill="#e5e5e5")


	def load_pattern(self):
		print("please Enter the pattern of your choice")
		pattern = input()
		f = open("./patterns/"+pattern, 'r') 
		if(not f):
			print("Failed to Load Pattern.")
			return
		p =  json.load(f)		
		#print(type(p["pattern"]))
		pattern_list  = p["pattern"]
		for p in pattern_list:
			col = p["column"]
			row = p["row"]
			if(self.check_validity(row,col)):
				self.game_grid[row][col] = 1
				
	def load_config(self):
		f = open("config.json", 'r') 
		if(not f):
			print("Failed to Load Config.")
			return
		config =  json.load(f)
		print(config)
		self.set_grid_params(config["grid_size"],config["game_speed"])

	def check_validity(self,i,j):
		return i >= 0 and j >= 0  and i < self.grid_size // 10 and j < self.grid_size // 10

	def process_level(self):
		aux_grid = np.zeros((self.grid_size,self.grid_size))

		
		dx = [1,-1,0]
		dy = [1,-1,0]
		print("Progessing To Next Level .....")
		for i in range(0,self.grid_size // 10):
			for j in range(0, self.grid_size // 10 ):
				sum = 0

				for x in dx:
					for y in dy:
						curr_y = i + y
						curr_x = j + x
						if(self.check_validity(curr_x,curr_y)):
							sum += self.game_grid[curr_y][curr_x]

				if( ( (sum == 3  or sum == 4) and self.game_grid[i][j] == 1) or ( ( sum == 3 ) and self.game_grid[i][j] == 0 ) ):
					aux_grid[i][j] = 1
					self.canvas.create_rectangle( [(i*10,j*10),(i*10+10,j*10+10)],fill="#222" )
				else:
					aux_grid[i][j] = 0
					self.canvas.create_rectangle( [(i*10,j*10),(i*10+10,j*10+10)],fill="#fff" )

		self.game_grid = aux_grid
		self.root.after(self.game_speed * 1000,self.process_level)

	
	def execute(self):
		self.load_config()
		self.load_pattern()

		self.root = tk.Tk()
		self.canvas = tk.Canvas(self.root, height=self.grid_size, width=self.grid_size, bg='#f5f5f5')

		self.canvas.pack(fill=tk.BOTH, expand=True)

		self.canvas.bind('<Configure>', self.create_grid)
		self.root.after(self.game_speed*1000,self.process_level)

		self.root.mainloop()



