class Sector {
  private int x1, y1, x2, y2;
  private color rectColor;
  
  
  public Sector(int x1, int y1, int x2, int y2) {
    this.x1 = x1;
    this.y1 = y1;
    this.x2 = x2;
    this.y2 = y2;
  }
  
  
  public void render() {
    fill(rectColor);
    stroke(255);
    rect(x1, y1, x2, y2);
  }
  
  
  public void setColor(color rectColor) {
    this.rectColor = rectColor;
  }
  
  
  public int getX1() {
    return x1;
  }
  
  
  public int getY1() {
    return y1;
  }
  
  
  public int getX2() {
    return x2;
  }
  
  
  public int getY2() {
    return y2;
  }
}
