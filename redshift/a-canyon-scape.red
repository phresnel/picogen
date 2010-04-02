scene{
    film-settings{color-scale:0.023; convert-to-srgb:1;}

    render-settings{
        prev-vol{
            width:800;
            height:500;
            samples-per-pixel:1;
            surface-integrator{
                type:redshift;
            }
            volume-integrator{
                type:none;
                step-size:1600;
                cutoff-distance:40000
            }
        }
        prev-no-vol{
            width:400;
            height:300;
            samples-per-pixel:1;
            surface-integrator{
                type:redshift;
            }
        }
    }
    cameras{
        aerial{
            transform{
                move-right{9000}
                move-up{8000}
                move-forward{-150000}

                pitch{7}
                roll{0}
            }
        }
    }
    objects{
        horizon-plane{
                color{rgb{0.5;0.5;0.5}}
                height:2020;
        }
        lazy-quadtree{
                color{rgb{0.6;0.4;0.2}}
                max-recursion:12;
                lod-factor:0.01;
                size:1000000;
                code:   /*(* 1000 (cos (* x 0.001)) (cos (* y 0.001)))*/
                        (defun foo (x y)
                                (^ (- 1 (abs ([LayeredNoise2d
                                        filter{cosine}
                                        seed{3}
                                        frequency{0.1}
                                        layercount{6}
                                        persistence{0.55}
                                ] (abs x) (abs y)))) 3)
                        )

                        (defun ridged (x y)
                                (- 1 (abs (^ (foo x y) 2)))
                        )

                        (+ -1500 (* 9000  (foo (* x 0.0001) (* 0.0001 y))))
                ;
        }
    }
    backgrounds{
        pss-sunsky {
                turbidity:2.7;
                sun-direction{3.0;0.1;0}
        }
    }
    volumes {
        exponential {
                absorption{rgb{0;0;0}}
                out-scatter{rgb{0.01;0.01;0.01}}
                emission{rgb{0.001;0.001;0.001}}
                min{0;1700;0}
                phase-function:0;
                base-factor:1;
                exponent-factor:0.009;
        }
    }
}
