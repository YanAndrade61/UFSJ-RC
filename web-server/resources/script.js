function toggleDetails(sectionId) 
{
    const detailsElement = document.getElementById(`${sectionId}-details`);
    if (detailsElement.style.display === 'block') {
        detailsElement.style.display = 'none';
    } else {
        detailsElement.style.display = 'block';
    }
}
