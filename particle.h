class Particle {

    private:
        bool operator==(const Particle& p) const {
            return row == p.row && col == p.col;
        }
        int row;
        int col;

        float x, y;
        float vx, vy;
        int state;
        long time_remaining;
        
    public:
        static const int QUIET = 1;
        static const int MOVE = 2;

        Particle();
        Particle(int row, int col);
        void set_position(int x,int y);
        void init_movement(int destination_x,int destination_y,int duration);
        void integrate(long t);
        void draw();
        int getState();
};