# pylint: disable=c0111

from nbhosting.courses import (
    Track, Section, Notebook,
    notebooks_by_patterns, track_by_directory)

def tracks(coursedir):
    """
    coursedir is a CourseDir object that points
    at the root directory of the filesystem tree
    that holds notebooks

    result is a list of Track instances
    """

    track_specs = [
        ('C++ avancé', 'C++ avancé', 'cplusplus', [
            ('1/9: intro', 'notebooks/1-*.md'),
            ('2/9: ???', 'notebooks/2-*.md'),
            ('3/9: ???', 'notebooks/3-*.md'),
            ('4/9: ???', 'notebooks/4-*.md'),
            ('5/9: ???', 'notebooks/5-*.md'),
            ('6/9: ???', 'notebooks/6-*.md'),
            ('7/9: ???', 'notebooks/7-*.md'),
            ('8/9: ???', 'notebooks/8-*.md'),
            ('9/9: ???', 'notebooks/9-*.md'),
        ])]

    return [Track(coursedir,
                  [Section(coursedir=coursedir,
                           name=section_name,
                           notebooks=notebooks_by_patterns(
                               coursedir, patterns))
                   for section_name, *patterns in section_specs],
                  name=track_name,
                  description=track_description,
                  id=track_id)
            for (track_name, track_description, track_id, section_specs) in track_specs]
