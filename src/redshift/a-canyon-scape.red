scene {
        film-settings{
                color-scale:0.00005;
                convert-to-srgb:0;
        }

        render-settings{
                prev-vol{
                        width:512;
                        height:512;
                        samples-per-pixel:5;
                        surface-integrator{
                                type:redshift;
                                ambient-samples:10;
                        }
                        volume-integrator{
                                type:none;
                                step-size:40;
                                cutoff-distance:1000000
                        }
                }
                prev-no-vol{
                        width:400;
                        height:300;
                        samples-per-pixel:1;
                        surface-integrator{
                                type:whitted;
                        }
                }
        }
        cameras{
                aerial{
                        front:1.1;
                        transform{
                                move-right{9000}
                                move-up{1450}
                                move-forward{-150000}

                                pitch{-6}
                                roll{0}
                        }
                }
                front{type:cubemap-front; transform{move{9000;1250;-150000}}}
                right{type:cubemap-right; transform{move{9000;1250;-150000}}}
                back{type:cubemap-back; transform{move{9000;1250;-150000}}}
                left{type:cubemap-left; transform{move{9000;1250;-150000}}}
                top{type:cubemap-top; transform{move{9000;1250;-150000}}}
                bottom{type:cubemap-bottom; transform{move{9000;1250;-150000}}}
        }
        objects{
                lazy-quadtree{
                        material{
                                color{
                                        type:rgb;
                                        rgb{1.0;1.0;1.0}
                                }
                        }
                        max-recursion:8;
                        lod-factor:0.01;
                        size:1000000;
                        code:   /*(* 1000 (cos (* x 0.001)) (cos (* y 0.001)))*/
                                (defun foo (x y)
									([LibnoiseRidgedMulti
                                                seed{3}
                                                frequency{0.1}
                                                octave-count{10}                                                
												lacunarity{1.9}
                                     ] (abs x) (abs y))
                                )

                                (defun ridged (x y)
                                 (- 1 (abs (^ (foo x y) 2)))
                                )

                                (+ -2500 (* 9000  (foo (* x 0.0001) (* 0.0001 y))))
                        ;
                }
        }
        backgrounds{
                pss-sunsky {
                        turbidity:2;
                        overcast:0.0;
                        sun-direction{1;0.35;0}
                        atmospheric-effects:0;
                        sun-brightness-factor:1;
                        atmospheric-effects-distance-factor:1.0;
                }
        }
        volumes {
                homogeneous {
                        absorption{type:rgb; rgb{5.15; 0.15; 0.15}}
                        out-scatter{type:rgb; rgb{0.125;0.125;0.125}}
                        emission{type:rgb; rgb{0.3;0.3;0.3}}
                        min{0;1020;0}
                        phase-function:0;
                        base-factor:1.0;
                        exponent-factor:0.015;
                        epsilon:0.01;
                }
        }
}
