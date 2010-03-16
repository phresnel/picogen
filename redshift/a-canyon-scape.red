scene{
    film-settings{color-scale:0.4; convert-to-srgb:0;}
    render-settings{
        prev-vol{
            width:512;
            height:171;
            samples-per-pixel:1;
            surface-integrator{
                type:redshift;
            }
            volume-integrator{
                type:emission;
                step-size:50;
                cutoff-distance:4000000;
            }
        }
        prev-no-vol{
            width:800;
            height:600;
            samples-per-pixel:1;
            surface-integrator{
                type:redshift;
            }
        }
    }
    cameras{
        aerial{
            transform{
                move-up{620}
                move-forward{8000}
                yaw{0}
                pitch{0}
            }
        }
    }
    objects{
        lazy-quadtree{
                color{1.0;1.0;1.0}
                max-recursion:10;
                lod-factor:0.000125;
                size:500000;
                code:
                        (defun foo (x y)
                                ([LayeredNoise2d
                                        filter{cosine}
                                        seed{5}
                                        frequency{0.0001}
                                        layercount{8}
                                        persistence{0.5}
                                ] (+ 100000 x) (+ 100000 y))
                        )

                        (+ 500 (* 5000  (foo x y)))
                        /*(+ 300 (* (sin (* x 0.001)) (sin (* y 0.001))))*/
                ;
        }
        /*water-plane{height:0; }*/
    }
    backgrounds{
        pss-sunsky {
                turbidity:2.2;
                sun-direction{0.0;0.5;0.0}
        }
    }
    volumes {
        homogeneous {
                absorption{0.002;0.002;0.002}
                out-scatter{0.0;0.0;0.0}
                emission{0.002;0.002;0.002}
                min{0;0;0}
                phase-function:0;
                base-factor:1;
                exponent-factor:0.005;
        }
    }
}
